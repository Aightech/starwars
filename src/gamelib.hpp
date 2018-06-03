#ifndef GAME_LIB_H
#define GAME_LIB_H


#include "network/netapi.hpp"

#include "GUI/guilib.hpp"
#include "player/playerlib.hpp"
//#include "elements/element.hpp"


#include <list>
#include <atomic>
#include <iostream>
#include <string>
#include <mutex>

using namespace sf;
using namespace std;

class GUI;

class Game: public  NetAPI
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

	/*! \brief testing method */
	void test();

	/*! \brief Process the 'menu' state of the game. It's mainly a event loop */
	//void menu();


	/*! \brief launch an online game. The game have to be setted (through menu() )*/
	void startGUI();

	/*! \brief Wait the gui thread to end.*/
	bool GUIactive(){return !m_guiTerminate;};

	/*! \brief Wait the gui thread to end.*/
	bool endGUI(){m_guiTerminate=true;};
	
	bool processRequest(Request* req);
	bool request(Request* req);
	
	bool sendUpdateAreaAround(Element * e);
	bool sendUpdate(Element * e);

	/*! \brief */
	void addElement(Element * element);


	/*! \brief */
	//TODO improve to tackle elmtListIndex Maximum and come back to 0 
	int elmtListIndex(){return m_elementsIndex;};

	//TODO: finish method
	void update();

	int processReceiverMessage(char * buffer, char* reply);
	
	bool isOnline(){return m_online;};
	void setOnline(int port);
	void setServer(int port);
	
	bool sendRequest(Request * req);
	
	int processPlayerRequest(char * buffer);
	int processServerUpdate(char * buffer);
	
	mutex* getElmtMtx(){return &m_elmtsMtx;}
	
	void setTurn(int playerNo);



	private:
	GUI* m_gui;
	thread* m_gui_thread;

	/*! \brief represent the map and all its elements (each pixel contain 0 or the ID of the element it shows)*/
	unsigned long int *m_map;
	int m_mapHeight;
	int m_mapWidth;

	vector<Player *> m_players;
	list<Element *> m_elements;
	int m_elementsIndex;
	int m_currentPlayer=0;

	atomic<bool> m_guiTerminate;
	mutex m_elmtsMtx;


	bool m_online=false;
	bool m_isServer=false;



};




#endif
