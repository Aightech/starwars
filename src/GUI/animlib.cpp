#include "animlib.hpp"
#include <iostream>

using namespace std;

Animation(const std::string s,const int nb, const int width){
	m_total_n=nb;
	m_width=width;
	m_state=0;
	Sprite * anim = new Sprite();
	if (!m_texture.loadFromFile(s))
		{
			cout << "Erreur chargement image!"<< endl;
			// return
		}

	anim->setTexture(t);
	anim->setTextureRect(sf::IntRect(0, 0, window.getSize().x,window.getSize().y));
	//m_arraySprite.push_back(logo);
	//setposition
}