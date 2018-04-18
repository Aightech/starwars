#ifndef GAME_LIB_H
#define GAME_LIB_H


//#include "netlib.hpp"

#include "GUI/guilib.hpp"
#include "elements/element.hpp"


#include <list>
#include <atomic>
#include <iostream>
#include <string>
#include <mutex>

using namespace sf;
using namespace std;


//struct to store players data
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
       
       /*! \brief Wait the gui thread to end.*/
       bool GUIactive(){return !m_guiTerminate;};
       
       /*! \brief Wait the gui thread to end.*/
       bool endGUI(){m_guiTerminate=true;};
       
       /*! \brief */
       void addElement(Element * element);
       
      
       /*! \brief */
       //TODO improve to tackle elmtListIndex Maximum and come back to 0 
       int elmtListIndex(){return m_elementsIndex;};
       
       //TODO: finish method
       void update();
       
       
       
       private:
       GUI* m_gui;
       thread* m_gui_thread;
       
       /*! \brief represent the map and all its elements (each pixel contain 0 or the ID of the element it shows)*/
       unsigned int *m_map;
       int m_mapHeight;
       int m_mapWidth;
       
       
       //TODO replace with a boost list
       list<Element *> m_elements;
       int m_elementsIndex;
       
       atomic<bool> m_guiTerminate;
       mutex m_elmtsMtx;
       
};




#endif
