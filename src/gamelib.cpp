
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
       m_map = new unsigned int[m_mapHeight*m_mapWidth] ();
       
       
       //////------- GUI SETTING ------- /////
       //allocate a new GUI and share with it the new map and its dimensions.
       m_gui = new GUI(m_map,m_mapWidth,m_mapHeight);
       
       
       
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

void Game::start()
{
       
       //DEBUG: set the context to debug the game mode
       m_gui->getState()=GAME_CONTEXT;
       
       m_guiTerminate=false;
       
       //start a new thread for the GUI
       m_gui_thread = new std::thread(&GUI::start, m_gui,(void*)this);
           
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
}


void Game::update()
{
       m_elmtsMtx.lock();
       for(list<Element*>::iterator it = m_elements.begin(); it !=  m_elements.end(); it++) 
       {
              m_elmtsMtx.unlock();
              (*it)->update();
              m_elmtsMtx.lock();
       }
       m_elmtsMtx.unlock();
       //cout << "----"<< endl;
}



