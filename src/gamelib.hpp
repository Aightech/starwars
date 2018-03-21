#ifndef GAME_LIB_H
#define GAME_LIB_H


//#include "netlib.hpp"

#include "GUI/guilib.hpp"
#include "elements/element.hpp"



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
       
       void addElement(Element * element);
       
       int elmtListIndex();
       
       
       
       
       
       private:
       GUI* m_gui;
       thread* m_gui_thread;
       
       unsigned int *m_map;
       int m_mapHeight;
       int m_mapWidth;
       
       Element * m_elements[10000];
       int m_elementsIndex;
       
};




#endif
