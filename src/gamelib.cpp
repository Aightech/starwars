
#include <iostream>
#include <string>

#include "netlib.hpp"
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
       m_mapHeight = MAP_HEIGHT;
       m_mapWidth = MAP_WIDTH;
       m_map = new unsigned int[m_mapHeight*m_mapWidth] ();
       
       m_gui = new GUI(m_map,m_mapWidth,m_mapHeight);
       
       for(int i =0; i<10000;i++)
              m_elements[i]=NULL;
       m_elementsIndex =0;
       
       Element::map()=m_map;
       Element::mapHeight()=m_mapHeight;
       Element::mapWidth()=m_mapWidth;

}


void Game::init()
{   
}

void Game::start()
{
       m_gui->getState()=GAME_CONTEXT;
       m_gui_thread = new std::thread(&GUI::start, m_gui,(void*)this);
           
}

void Game::addElement(Element * element)
{
       
}

int Game::elmtListIndex()
{
       int index = 0;
       while(m_elements[index]==NULL){index++;}
       return index;
}





