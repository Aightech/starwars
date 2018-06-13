
#include <iostream>
#include <string>

//#include "GUI/guilib.hpp"
#include "gamelib.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>






using namespace sf;
using namespace std;

int WATCHPORT;

int parseNumber(char * buffer, char key, int ifNot)
{
	return ((buffer=strchr(buffer,key))!=NULL)?atoi(buffer+1):ifNot;
}

Game::Game()
{

	//////------- MAP SETTING ------- /////
	//set the map dimensions thanks to the macros define in macro.txt file
	m_mapHeight = MAP_HEIGHT;
	m_mapWidth = MAP_WIDTH;
	//allocate the int array represending the map 
	m_map = new unsigned long int[m_mapHeight*m_mapWidth] ();

	//default setting
	m_online = false;
	m_isServer = false;
	m_elementsIndex =0;

	//////------- ELEMENT SETTING ------- /////
	Element::map()=m_map;
	Element::mapHeight()=m_mapHeight;
	Element::mapWidth()=m_mapWidth;

	//Need to set all the element used in the game
	Headquarter::setting();
	Warehouse::setting();
	Tower::setting();
	Farm::setting();
	Unit::setting();
}


void Game::startGUI()
{

	//////------- GUI SETTING ------- /////
	//allocate a new GUI and share with it the new map and its dimensions.
	m_gui = new GUI(m_map,m_mapWidth,m_mapHeight);

	//DEBUG: set the context to debug the game mode
	m_gui->getState()=MAIN_MENU;//GAME_CONTEXT;

	m_guiTerminate=false;

	//start a new thread for the GUI
	m_gui_thread = new std::thread(&GUI::start, m_gui,(void*)this);

}

void Game::setOnline(int port)
{
	setConnectionPhrase((char*)CONNECTION_PHRASE);
	//start the network UDP listener inerited from netapi
	startReceiver(port,(char*)"UDP");
	m_online = true;
}

void Game::setServer(int port)
{
	setConnectionPhrase((char*)CONNECTION_PHRASE);
	//start the network TCP listener inerited from netapi
	startReceiver(port,(char*)"TCP");
	m_online = true;
	m_isServer = true;
}

void Game::addElement(Element * element)
{
	if(m_verbose)
	{
		cout << "Element added" << endl;
		cout << element->no() <<  endl;
		cout << "++++++++++++" << endl;
	}
	//add the element to the element array of the game
	m_elementsIndex++;
	m_elmtsMtx.lock();
	m_elements.push_back(element);
	m_elmtsMtx.unlock();
}

Player* Game::addPlayer(int no)
{
	//add a player to the array player of the game
	m_players.push_back(new Player(this,no));
	cout << "added player " << endl;
	
	//if main game, add a headquater to the player
	if(!m_online || m_isServer)
	{
		Request req={HEADQUARTER_TYPE,20 +(m_mapWidth-180)*no,20 +(m_mapHeight-125)*no,-1,0,(unsigned long int)no};
		std::cout << "HEADQUARTER" << std::endl;
		processRequest(&req);
	}
	return m_players.back();
}

bool Game::sendRequest(Request * req)
{
	char buffReq[1024],reply[1024];
	//format of a request
	//|   M   |   R   |    int    |   E   | int  |  U  | int |  V  | int |  P   | int
	//|message|request|type of req|Element|numero|pos X|value|pos Y|value|player|numero
	sprintf(buffReq,"MR%dE%dU%dV%dP%d",req->type,(req->e!=0)?((Element *)req->e)->no():-1,req->val1,req->val2,(int)req->p);
	//send the request to the server
	this->sendToServer(buffReq,(char*)"tcp",reply);
	//return true if the server received the req
	return (strcmp(reply,"rcvd") == 0)?true:false;
}

bool Game::request(Request* req)
{
	//if the game is online and not the server it cannot process the req itself and thus have to send it to the server
	if(m_online && !m_isServer)
	{//only send not empty request
		if(req->type!=NO_REQUEST)
			sendRequest(req);
	}
	else
	{//if if the game is server or offline it can process the request itself
		processRequest(req);
	}
	return true;
}

bool Game::processRequest(Request* req)
{
	if(req->type == NO_REQUEST)
		return false;
	//request of element creation
	if(req->type/NB_MAX_ELEMENT==0)
	{
		r_creationElement(req);
	}
	else//if the request is an action
	{
		cout<< "try smt"  << req->type <<endl;
		Element * e = findElement(req->e);
		if(e!=NULL)
		{
			switch(req->type)
			{
				case R_MOVE://move the unit
				{
					m_elmtsMtx.lock();
					cout<< "move" <<endl;
					if(((Unit *)e)->move(req->val1,req->val2)==1)
						sendUpdateAreaAround(((Unit *)(e)));
					m_elmtsMtx.unlock();
					cout<< "end move" <<endl;
				}break;
				case R_TARGET://change the target of a element
				{
					m_elmtsMtx.lock();
					e->setTarget(req->val1,req->val2);
					m_elmtsMtx.unlock();
				}break;

				case R_ACTION:
				case R_HEAL:
				case R_ATTACK:
				{
					m_elmtsMtx.lock();
					int hp =e->getDamage(req->val1);
					m_elmtsMtx.unlock();
					if(hp<0)
					{
						cout << "killing" << endl;
						m_players[e->player()->no()]->removeElement((unsigned int)req->e);
						list<Element*>::iterator it = m_elements.begin();
						while(it !=  m_elements.end() && (*it) != e) {it++;}
						delete e;
						m_elements.erase(it);
					}
				}break;
			}
		}
	}
	return true;
}

Element * Game::findElement(int no)
{
	if(no == -1)
		return NULL;
	m_elmtsMtx.lock();
	list<Element*>::iterator it = m_elements.begin();
	while(it !=  m_elements.end() && (*it)->no() != no) 
	{
		m_elmtsMtx.unlock();
		m_elmtsMtx.lock();
		it++;
	}
	m_elmtsMtx.unlock();
	if(it !=  m_elements.end() && (*it)->no() == no)
		return (*it);
	else
		return NULL;
}


bool Game::r_creationElement(Request *req)
{
	//get the player of the new element by its numero
	Player * p = (req->p==-1)?m_players[0]:m_players[req->p];
	
	/*Process to find player in a game with more than two player
	if(req->p==-1)
		p = m_players[0];
	else 
	{
		int i =0;
		while( i < m_players.size() && req->p != m_players[i]->no()){i++;}
		if(i < m_players.size() && req->p == m_players[i]->no())
			p = m_players[i];
		else
			p=addPlayer(req->p);
	}*/
	//allocate the new element thanks two the static method factory that return a element of the right type
	Element * e= Element::factory(req->type, (req->e==0)?m_elementsIndex:(req->e), req->val1, req->val2, p);
	if(e==NULL)	return false;
	if(req->val3 != -1)//if hp has been specified 
		e->HP() = req->val3;
	//add the element to the game array
	p->addElement(e);
	//add the element to the right player
	this->addElement(e);
	
	//update the area around the new element
	sendUpdateAreaAround(e);
}


bool Game::sendUpdate(Element * e)
{
	char updateMsg[512];
	
	//format of an update 
	//|   M   |   U   |   E   | int  | T  |  int   |  X  | int |  Y  | int |  H  | int |  P   | int
	//|message|update |Element|numero|type|TYPE_VAL|pos X|value|pos Y|value|  HP |value|player|numero
	sprintf(updateMsg,"MUE%dT%dX%dY%dH%dP%d", e->no(), e->type(), e->x(), e->y(), e->HP(),e->player()->no());
	sendToClient(ALL_CLIENT,updateMsg);
}

bool Game::sendUpdateAreaAround(Element * e)
{
	if(!m_online)
		return false;
		
	//get element infos
	int e_height = e->height();
	int e_width  = e->width();
	int e_y = e->y();
	int e_x = e->x();
	int i_start = (e_y - e_height >= 0)?e_y - e_height:0;
	int j_start = (e_x - e_width >= 0)?e_x - e_width:0;
	int i_end = (e_y + e_height < m_mapHeight)?e_y + e_height:m_mapHeight;
	int j_end = (e_x + e_width  < m_mapWidth )?e_x + e_width: m_mapWidth ;
	
	//look for other ellement in the area around it
	for(int i = i_start; i < i_end+4 ; i+=5 )
		for(int j = j_start; j < j_end+4 ; j+=5 )
			if(m_map[i*m_mapWidth + j] != 0)
				sendUpdate((Element*) m_map[i*m_mapWidth + j]);
	return true;
}


void Game::update()
{
	//if the server have a new client it have to had a player
	if(m_isServer && m_players.size() < m_clientIndex)
	{
		m_players.push_back(new Player(this,m_players.size()));
	}
	
	//mode offline
	if(!m_online)
	{
		for(int i =0; i<m_players.size(); i++)
			if(m_players[i]->turn())
				m_players[i]->update(50000);
	}
	//mode online
	else if(m_isServer)//the server update continously each player
		for(int i =0; i<m_players.size(); i++)
			m_players[i]->update(1);
	else//the client process the update of the server
	{
		char enter[1024];
		if(getReceiverBuffer(enter)>-1)
		{
			processServerUpdate(enter);
		}
	}
}

void Game::setTurn(int playerNo)
{
	if(playerNo<m_players.size())
		m_players[playerNo]->turn()=1;
}

int Game::processReceiverMessage(char * buffer, char* reply)
{
	strcpy(reply,"rcvd");
}

int Game::processPlayerRequest(char * buffer)
{
	Request r = {parseNumber(buffer,'R',NO_REQUEST),parseNumber(buffer,'U',10),parseNumber(buffer,'V',10),parseNumber(buffer,'W',-1)};
	r.p = parseNumber(buffer,'P',-1);
	processRequest(&r);
}

int Game::processServerUpdate(char * buffer)
{
	if(buffer[1] == 'U' && buffer[2] == 'E')
	{
		int no = parseNumber(buffer,'E',-1);
		
		if(no == -1)
			return -1;
		
		//find the element in the list and update it
		for(list<Element*>::iterator it = m_elements.begin(); it !=  m_elements.end(); it++) 
		{
			if((*it)->no()== no)
			{
				(*it)->updateStatut(	parseNumber(buffer,'X',(*it)->x()),
							parseNumber(buffer,'Y',(*it)->y()),
							parseNumber(buffer,'H',(*it)->HP())	);
				return 1;
			}
		}
		//if the element hasn't been found it have to be create
		Request r = {	parseNumber(buffer,'T',NO_REQUEST),
				parseNumber(buffer,'X',10),
				parseNumber(buffer,'Y',10),
				parseNumber(buffer,'H',-1),
				(unsigned long int )no	,
				(unsigned long int )parseNumber(buffer,'P',-1)	};
		processRequest(&r);
		return 1;
		
	}
	return -1;
}


