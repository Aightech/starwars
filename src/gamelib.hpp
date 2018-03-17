#ifndef GAME_LIB_H
#define GAME_LIB_H



//#define WATCHPORT 4001

#include "netlib.hpp"
#include "GUI/guilib.hpp"
#include "gamelib.hpp"

#include <thread>
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

typedef struct _Player {

	int no;
	char name[16];
	char IPaddress[16];
	int  portNo;
	
	int card[6];
	
}Player;





class Game
{
       /*! \class Game
       * \brief This class represent the game application.
       *      It owns the different methods running the game.
       */
       public:
       
       /*! \brief Constructor */
       Game();
       
       /*! \brief Destructor */
       ~Game(){};
       
       /*! \brief Initialization methode. Some parametere are initialize inside */
       void init();
       
       /*! \brief Process the 'menu' state of the game. It's mainly a event loop */
       //void menu();
       
       
       /*! \brief launch an online game. The game have to be setted (through menu() )*/
       void start();
       
       void end(){m_gui_thread->join();};
       
       
       
       
       
       private:
       GUI* m_gui;
       thread* m_gui_thread;
       
       
       
       
       
       
        
       
};




#endif
