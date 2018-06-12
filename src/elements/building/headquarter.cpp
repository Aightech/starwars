#include "headquarter.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

const Color Headquarter::s_color = HEADQUARTER_COLOR;
const Texture* Headquarter::s_texture = new Texture();

void Headquarter::setting()
{
	Headquarter::setTexture();
	Element::elements()[HEADQUARTER_TYPE] = new Headquarter();
};

Headquarter::Headquarter(int no, int px, int py,  Player * player): Element(no,px,py,player)
{
	m_width = s_width;
	m_height = s_height;
	m_HP = s_HP;
	m_defense = s_defense;
	m_type = s_type;
	m_color = s_color;

	m_sprite.setTexture(*s_texture);
	m_sprite.setTextureRect(sf::IntRect(0, ((no==-1)?(unsigned long int)player:player->no())*(m_height), m_width, m_height));
	//m_sprite.setScale(Vector2f(0.9,0.9));
	m_sprite.setPosition(Vector2f(s_mapOffsetX+m_x,s_mapOffsetY + m_y));
	//m_sprite.setTextureRect(sf::IntRect(0, 0,m_width, m_height));
	//m_sprite.setColor(m_color);
	
	if(no!=-1)
		updatePos();
}



Request Headquarter::update()
{
	Request req;
	return req;
}
