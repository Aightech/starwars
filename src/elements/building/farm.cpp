#include "farm.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

const Color Farm::s_color = FARM_COLOR;
const Texture* Farm::s_texture = new Texture();
const char Farm::s_pathButtonTexture[] = FARM_PATH_BUTTON_TEXTURE;
const char Farm::s_pathButtonTexture2[] = FARM_PATH_BUTTON_TEXTURE2;

void Farm::setting()
{
	Farm::setTexture();
	Element::elements()[FARM_TYPE] = new Farm();
};

Farm::Farm(int no, int px, int py, Player * player): Buildable(no,px,py,player)
{ 
	m_isBuidable = true;
	m_width = s_width;
	m_height = s_height;
	m_HP = s_HP;
	m_defense = s_defense;
	m_type = s_type;
	m_color = s_color;
	m_nbpop = s_nbpop;
	
	m_pathButtonTexture = s_pathButtonTexture;
	m_pathButtonTexture2 = s_pathButtonTexture2;

	cout << "ok farm" << endl;
	m_sprite.setTexture(*s_texture);
	//m_sprite.setScale(Vector2f(0.25,0.25));
	m_sprite.setPosition(Vector2f(s_mapOffsetX+m_x,s_mapOffsetY + m_y));
	m_sprite.setTextureRect(sf::IntRect(0, ((no==-1)?(unsigned long int)player:player->no())*(m_height), m_width, m_height));
	cout << "ok" << endl;
	if(player!=NULL)
		player->populationMax()+=s_nbpop;

	updatePos();
	cout << "ok2" << endl;
};

void Farm::setTexture()
{
	Image im;
	if (!im.loadFromFile("media/elements/farm.png")){
		cout << "Erreur chargement image!"<< endl;
	}
	const_cast<Texture*>(s_texture)->loadFromImage(im);
}


