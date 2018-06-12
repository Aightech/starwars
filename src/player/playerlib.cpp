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
	m_turn =0;
//	Request req={HEADQUARTER_TYPE,0,0,0,0,(unsigned long int)no};
//	std::cout << "HEADQUARTER" << std::endl;
//	m_game->request(&req);
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
	m_turn=0;
	
};

void Player::addElement(Element * element)
{
	m_elementsIndex++;
	m_elmtsMtx->lock();
	this->m_elements.push_back(element);
	m_elmtsMtx->unlock();
}

void Player::removeElement(int no)
{
	m_elmtsMtx->lock();
	list<Element*>::iterator it = m_elements.begin();
	while(it !=  m_elements.end() && (*it)->no() != no) 
	{
		m_elmtsMtx->unlock();
		m_elmtsMtx->lock();
		it++;
	}
	m_elmtsMtx->unlock();
	m_elements.erase(it);
}



