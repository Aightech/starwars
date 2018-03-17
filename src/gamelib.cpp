
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
       m_gui = new GUI();

}


void Game::init()
{   
}

void Game::start()
{
       m_gui->getState()=GAME_CONTEXT;
       m_gui_thread = new std::thread(&GUI::start, m_gui);
       
}


