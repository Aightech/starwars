#ifndef PLAYER_LIB_H
#define PLAYER_LIB_H


#include "../elements/element.hpp"
#include "../GUI/guilib.hpp"


#include <list>
#include <atomic>
#include <iostream>
#include <string>
#include <mutex>

using namespace sf;
using namespace std;


//struct to store players data
/*typedef struct _Player {

	int no;
	char name[16];
	char IPaddress[16];
	int  portNo;
	
	int card[6];
	
}Player;*/





class Player
{
       /*! \class Game
       * \brief This class represent the game application.
       *      It owns the different methods running the game.
       */
       public:
       
       /*! \brief Constructor */
       Player();
       
       /*! \brief Destructor */
       ~Player(){};
       
       /*! \brief testing method */
       void test();
       
       
       
       //TODO: finish method
       void update();
       
       
       
       private:
       
       /*! \brief represent the map and all its elements (each pixel contain 0 or the ID of the element it shows)*/
       unsigned int *m_map;
       int m_mapHeight;
       int m_mapWidth;
       
       
       list<Element *> m_elements;
       int m_elementsIndex;
       
       
       
};




#endif
