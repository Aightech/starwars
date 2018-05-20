#include <iostream>
#include <string>

//#include "netlib.hpp"
//#include "GUI/guilib.hpp"
#include "playerlib.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>





using namespace sf;
using namespace std;


Player::Player(Game *game,int no)
{
	m_game = game;
	m_elmtsMtx = game->getElmtMtx();
	m_no =no;
};


//No current use
void Player::test()
{   
}






void Player::update(int step)
{
	Request r={0,0,0,0,0,0};
	r.p=(unsigned long int)m_no;
	clock_t start= clock();
	for (int i = 0; i < step; i += 1)
	{
		m_elmtsMtx->lock();
		for(list<Element*>::iterator it = m_elements.begin(); it !=  m_elements.end(); it++) 
		{
			m_elmtsMtx->unlock();
			m_elmtsMtx->lock();
			r = (*it)->update();
			m_elmtsMtx->unlock();
			m_game->request(&r);
			m_elmtsMtx->lock();
		}
		m_elmtsMtx->unlock();
		
	}
	
};

void Player::addElement(Element * element)
{
	m_elementsIndex++;
	m_elmtsMtx->lock();
	this->m_elements.push_back(element);
	m_elmtsMtx->unlock();
}



