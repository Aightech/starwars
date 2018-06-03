
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

	m_online = false;
	m_isServer = false;

	m_elementsIndex =0;
	
	m_players.push_back(new Player(this,0));
	m_players.push_back(new Player(this,1));
	
	//m_players[0]->turn()=1;


	//////------- ELEMENT SETTING ------- /////
	Element::map()=m_map;
	Element::mapHeight()=m_mapHeight;
	Element::mapWidth()=m_mapWidth;
	Warehouse::setting();
	Tower::setting();
	Farm::setting();
	Unit::setting();
	
}

//No current use
void Game::test()
{
	list<Element*>::iterator it = m_elements.begin();
	(*it)->getDamage(1);
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
	startReceiver(port,(char*)"UDP");
	m_online = true;
}

void Game::setServer(int port)
{
	setConnectionPhrase((char*)CONNECTION_PHRASE);
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
	 m_elementsIndex++;
	m_elmtsMtx.lock();
	m_elements.push_back(element);
	m_elmtsMtx.unlock();


}

bool Game::sendRequest(Request * req)
{
	char buffReq[1024],reply[1024];
	sprintf(buffReq,"MR%dE%dU%dV%dP%d",req->type,(req->e!=0)?((Element *)req->e)->no():-1,req->val1,req->val2,(int)req->p);
	this->sendToServer(buffReq,(char*)"tcp",reply);
	return (strcmp(reply,"rcvd") == 0)?true:false;
}

bool Game::request(Request* req)
{
	if(m_online && !m_isServer)
	{
		if(req->type!=NO_REQUEST)
			sendRequest(req);
	}
	else
	{
		processRequest(req);
	}
	return true;
}

bool Game::processRequest(Request* req)
{
	if(req->type == NO_REQUEST)
		return false;
	if(req->type/NB_MAX_ELEMENT==0)//request of element creation
	{
		Player * p = (req->p==-1)?m_players[0]:m_players[req->p];
		Element * e= Element::factory(req->type, (req->e==0)?m_elementsIndex:(req->e), req->val1, req->val2, p);
		if(e==NULL)	return false;
		if(req->val3 != -1)
			e->HP() = req->val3;
		p->addElement(e);
		this->addElement(e);
		sendUpdateAreaAround(e);
	}
	else
	{
		switch(req->type)
		{
			case R_MOVE:
			{
				if(((Unit *)(req->e))->move(req->val1,req->val2)==1)
					sendUpdateAreaAround(((Unit *)(req->e)));
			}break;

			case R_ACTION:
			case R_HEAL:
			case R_ATTACK:
			{
				m_elmtsMtx.lock();
				list<Element*>::iterator it = m_elements.begin();
				while(it !=  m_elements.end() && (*it)->no() != (unsigned int)req->e) 
				{
					m_elmtsMtx.unlock();
					m_elmtsMtx.lock();
					it++;
				}
				int hp =(*it)->getDamage(req->val1);
				m_elmtsMtx.unlock();
				if(hp<0)
				{
					cout << "killing" << endl;
					m_players[(*it)->player()->no()]->removeElement((unsigned int)req->e);
					delete (*it);
					m_elements.erase(it);
				}
				

			}break;
			case R_KILL:
			{
				m_elmtsMtx.lock();
				list<Element*>::iterator it = m_elements.begin();
				while(it !=  m_elements.end() && (*it)->no() != (unsigned int)req->e) 
				{
					m_elmtsMtx.unlock();
					m_elmtsMtx.lock();
					it++;
				}
				//m_players[(*it)->player->no()]->removeElement((unsigned int)req->e);
				delete (*it);
				m_elements.erase(it);
				m_elmtsMtx.unlock();

			}break;
		}
	}
	
	return true;
}

bool Game::sendUpdate(Element * e)
{
	char updateMsg[512];
	sprintf(updateMsg,"MUE%dT%dX%dY%dH%dP%d", e->no(), e->type(), e->x(), e->y(), e->HP(),e->player()->no());
	sendToClient(ALL_CLIENT,updateMsg);
}

bool Game::sendUpdateAreaAround(Element * e)
{
	int e_height = e->height();
	int e_width  = e->width();
	int e_y = e->y();
	int e_x = e->x();
	int i_start = (e_y - e_height >= 0)?e_y - e_height:0;
	int j_start = (e_x - e_width >= 0)?e_x - e_width:0;
	int i_end = (e_y + e_height < m_mapHeight)?e_y + e_height:m_mapHeight;
	int j_end = (e_x + e_width  < m_mapWidth )?e_x + e_width: m_mapWidth ;
	
	
	for(int i = i_start; i < i_end+4 ; i+=5 )
		for(int j = j_start; j < j_end+4 ; j+=5 )
			if(m_map[i*m_mapWidth + j] != 0)
				sendUpdate((Element*) m_map[i*m_mapWidth + j]);
}


void Game::update()
{
	if(!m_online)
	{
		for(int i =0; i<m_players.size(); i++)
			if(m_players[i]->turn())
				m_players[i]->update(50000);
	}
	else if(m_isServer)
		for(int i =0; i<m_players.size(); i++)
			m_players[i]->update(1);
	else
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
			
		for(list<Element*>::iterator it = m_elements.begin(); it !=  m_elements.end(); it++) 
		{
			if((*it)->no()== no)
			{
//				(*it)->x()  = parseNumber(buffer,'X',(*it)->x());
//				(*it)->y()  = parseNumber(buffer,'Y',(*it)->y());
//				(*it)->HP() = parseNumber(buffer,'H',(*it)->HP());
				(*it)->updateStatut(parseNumber(buffer,'X',(*it)->x()), parseNumber(buffer,'Y',(*it)->y()), parseNumber(buffer,'H',(*it)->HP()));
				return 1;
			}
		}
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


