#ifndef PLAYER_LIB_H
#define PLAYER_LIB_H

#include "../gamelib.hpp"
#include "../elements/element.hpp"
//#include "../GUI/guilib.hpp"



#include <list>
#include <atomic>
#include <iostream>
#include <string>
#include <mutex>

using namespace sf;
using namespace std;

class Game;


class Player
{
	/*! \class Game
	* \brief This class represent the game application.
	*      It owns the different methods running the game.
	*/
	public:

	/*! \brief Constructor */
	Player(Game *game,int no =-1);

	/*! \brief Destructor */
	~Player(){};

	/*! \brief testing method */
	void test();
	int& population(){return m_pop;}
	int& populationMax(){return m_popMax;}



	//TODO: finish method
	void update(int step=1);
	void addElement(Element * element);
	void removeElement(int no);
	int no(){return m_no;};
	int& turn(){return m_turn;};



	private:

	/*! \brief represent the map and all its elements (each pixel contain 0 or the ID of the element it shows)*/
	unsigned int *m_map;
	int m_mapHeight;
	int m_mapWidth;

	Game *m_game;
	int m_no;
	int m_pop=0;
	int m_popMax=5;
	int m_crystal;
	int m_turn;


	list<Element *> m_elements;
	int m_elementsIndex=0;
	mutex* m_elmtsMtx;


};




#endif
