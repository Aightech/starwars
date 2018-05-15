
#include <iostream>
#include <string>

#include "GUI/guilib.hpp"
#include "gamelib.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

/*#include "vector"
#include <time.h> 


#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/wait.h>

#include <netdb.h>
#include <arpa/inet.h>

#include <fcntl.h>

#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>*/




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


	//////------- ELEMENT SETTING ------- /////
	Element::map()=m_map;
	Element::mapHeight()=m_mapHeight;
	Element::mapWidth()=m_mapWidth;
	Warehouse::setting();
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
	m_gui->getState()=GAME_CONTEXT;

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
	cout << "Element added" << endl;
	cout << element->no() <<  endl;

	cout << "++++++++++++" << endl;
	 m_elementsIndex++;
	m_elmtsMtx.lock();
	m_elements.push_back(element);
	m_elmtsMtx.unlock();
	cout << "++++++++++++" << endl;


}

bool Game::sendRequest(Request * req)
{
	char buffReq[1024],reply[1024];
	sprintf(buffReq,"MR%dE%dU%dV%d",req->type,(req->e1!=0)?((Element *)req->e1)->no():-1,req->val1,req->val2);
	this->sendToServerTCP(buffReq,reply);
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
		Element * e= Element::factory(req->type, (req->e1==0)?m_elementsIndex:(req->e1), req->val1, req->val2);;
		if(e==NULL)	return false;
		if(req->val3 != -1)
			e->HP() = req->val3;
		this->addElement(e);
	}
	else
	{
		switch(req->type)
		{
			case R_MOVE:
			{
				((Unit *)(req->e1))->move(req->val1,req->val2);
			}break;

			case R_ACTION:
			case R_HEAL:
			case R_ATTACK:
			{

			}break;
		}
	}
	
	return true;
}


void Game::update()
{
	if(!m_online || m_isServer)
	{
		Request r={0,0,0,0};
		m_elmtsMtx.lock();
		for(list<Element*>::iterator it = m_elements.begin(); it !=  m_elements.end(); it++) 
		{
			m_elmtsMtx.unlock();

			r = (*it)->update();

			this->request(&r);
			if(m_isServer)
			{
				char up[1024],buff[1024];
				sprintf(up,"MUE%dT%dX%dY%dH%d",(*it)->no(),(*it)->type(),(*it)->x(),(*it)->y(),(*it)->HP());
				for(int i = 0; i < getClientAddr().size(); i++)
					sendToClientUDP(i,up);
			}
			m_elmtsMtx.lock();
		}
		m_elmtsMtx.unlock(); 
	}
	else
	{
		char enter[1024];
		if(getReceiverBuffer(enter)>-1)
		{
			processServerUpdate(enter);
		}
	}
}

int Game::processReceiverMessage(char * buffer, char* reply)
{
	strcpy(reply,"rcvd");
}

int Game::processPlayerRequest(char * buffer)
{
	Request r = {parseNumber(buffer,'R',NO_REQUEST),parseNumber(buffer,'U',10),parseNumber(buffer,'V',10),parseNumber(buffer,'W',-1)};
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
				//std::cout << "element:"<< (*it)->no() << std::endl;
				return 1;
			}
			//std::cout << "element:"<< (*it)->no() << std::endl;
		}
		//std::cout << "new element:"<< no << std::endl;
		Request r = {	parseNumber(buffer,'T',NO_REQUEST),
				parseNumber(buffer,'X',10),
				parseNumber(buffer,'Y',10),
				parseNumber(buffer,'H',-1),
				(unsigned long int )no		};
		processRequest(&r);
		return 1;
		
	}
	return -1;
}


