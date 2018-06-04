#include "animlib.hpp"
#include <iostream>

using namespace std;

Animation::Animation(std::string s, int px, int py, int width, int height, int nbFrame, double frameRate): m_x(px), m_y(py), m_width(width), m_height(height), m_nbFrame(nbFrame), m_frameRate(frameRate){
	m_state=0;
	m_texture = new Texture();
	if (!const_cast<Texture*>(m_texture)->loadFromFile(s))
	{
		cout << "Erreur chargement image!"<< endl;
		// return
	}
	m_ownTexture=true;
	m_sprite.setTexture(*m_texture);
	if(m_width!=-1)
		m_sprite.setTextureRect(sf::IntRect(0, 0, m_width,m_height));
	m_sprite.setPosition(Vector2f(m_x, m_y));
	
	m_clock = clock();
	//m_arraySprite.push_back(logo);
	//setposition
}

Animation::Animation(Texture* texture, int px, int py,  int width, int height, int nbFrame, double frameRate): m_x(px), m_y(py), m_width(width), m_height(height), m_nbFrame(nbFrame), m_frameRate(frameRate){
	m_state=0;
	m_texture = texture;

	m_sprite.setTexture(*m_texture);
	if(m_width!=-1)
		m_sprite.setTextureRect(sf::IntRect(0, 0, m_width,m_height));
	m_sprite.setPosition(Vector2f(m_x, m_y));
	
	m_clock = clock();
	//m_arraySprite.push_back(logo);
	//setposition
}

void Animation::set(const Texture* texture, int px, int py,  int width, int height, int nbFrame, double frameRate)
{
	m_x=px;
	m_y=py;
	m_width=width;
	m_height=height;
	m_nbFrame=nbFrame;
	m_frameRate=frameRate;
	m_state=0;
	m_texture = texture;

	m_sprite.setTexture(*m_texture);
	if(m_width!=-1)
		m_sprite.setTextureRect(sf::IntRect(0, 0, m_width,m_height));
	m_sprite.setPosition(Vector2f(m_x, m_y));
	
	m_clock = clock();
	//m_arraySprite.push_back(logo);
	//setposition
}



int Animation::update(RenderWindow &w)
{
	if(m_nbFrame >0 && clock()-m_clock>CLOCKS_PER_SEC*m_frameRate)
	{
		m_clock = clock();
		m_state = (m_state + 1)%m_nbFrame;
		m_sprite.setTextureRect(sf::IntRect(m_width*(m_state),0, m_width, m_height));
	}
	w.draw(m_sprite);
	return m_state;
}

void Animation::setPosition(sf::Vector2f position)
{
	m_sprite.setPosition(position);
}

void Animation::setScale(float sx, float sy)
{
	m_sprite.setScale(sx,sy);
}
