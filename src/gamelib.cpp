
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


Game::Game()
{
       
       //////------- MAP SETTING ------- /////
       //set the map dimensions thanks to the macros define in macro.txt file
       m_mapHeight = MAP_HEIGHT;
       m_mapWidth = MAP_WIDTH;
       //allocate the int array representing the map 
       m_map = new unsigned long int[m_mapHeight*m_mapWidth] ();
       
       
       
       m_elementsIndex =0;
       
       
       //////------- ELEMENT SETTING ------- /////
       Element::map()=m_map;
       Element::mapHeight()=m_mapHeight;
       Element::mapWidth()=m_mapWidth;

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
       startReceiver(port);
       m_online = true;
}

void Game::addElement(Element * element)
{
	cout << "Element added" << endl;
	cout << m_elementsIndex <<  endl;

	cout << "++++++++++++" << endl;

	m_elmtsMtx.lock();
	m_elementsIndex++;
	m_elements.push_back(element);
	m_elmtsMtx.unlock();
	cout << "++++++++++++" << endl;


}

void Game::sendRequest(Request * req,Element * elmt)
{
	char buffReq[1024];
	sprintf(buffReq,"MR%dE%dV%dW%d",req->type,elmt->no(),req->val1,req->val2);
	this->sentToServer(buffReq);
}

bool Game::request(Request* req,Element * elmt)
{
	if(m_online)
	{
		if(req->type!=NO_REQUEST)
			sendRequest(req,elmt);
	}
	else
	{
		switch(req->type)
		{
			case NO_REQUEST:     break;

			case R_CREATE_BUILDING:
			case R_CREATE_WAREHOUSE:
			case R_CREATE_FARM:
			case R_CREATE_TOWER:
			{

			}break;

			case R_CREATE_UNIT:
			case R_CREATE_SUPERUNIT:
			{
				switch(req->type)
				{
					case R_CREATE_UNIT:
					{
						cout << "request Unit"<< endl;
						this->addElement(new Unit(m_elementsIndex,req->val1,req->val2));


					}break;
					case R_CREATE_SUPERUNIT:
					{

					}break;
				}
			}break;

			case R_MOVE:
			{
				((Unit *)elmt)->move(req->val1,req->val2);

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
       Request r={0,0,0};
       m_elmtsMtx.lock();
       for(list<Element*>::iterator it = m_elements.begin(); it !=  m_elements.end(); it++) 
       {
              m_elmtsMtx.unlock();
              
              r = (*it)->update();
              
              this->request(&r,(*it));
              
              m_elmtsMtx.lock();
       }
       m_elmtsMtx.unlock(); 
       //cout << "----"<< endl;
}



