#include "warehouse.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

const Color Warehouse::s_color = WAREHOUSE_COLOR;
const Texture* Warehouse::s_texture = new Texture();
const char Warehouse::s_pathButtonTexture[] = WAREHOUSE_PATH_BUTTON_TEXTURE;
const char Warehouse::s_pathButtonTexture2[] = WAREHOUSE_PATH_BUTTON_TEXTURE2;

void Warehouse::setting()
{
	Warehouse::setTexture();
	Element::elements()[WAREHOUSE_TYPE] = new Warehouse();
};

Warehouse::Warehouse(int no, int px, int py,  Player * player): Buildable(no,px,py,player)
{
	m_isBuidable = true;
	m_width = s_width;
	m_height = s_height;
	m_HP = s_HP;
	m_defense = s_defense;
	m_type = s_type;
	m_color = s_color;
	m_pathButtonTexture = s_pathButtonTexture;
	m_pathButtonTexture2 = s_pathButtonTexture2;

	m_sprite.setTexture(*s_texture);
	m_sprite.setTextureRect(sf::IntRect(0, ((no==-1)?(unsigned long int)player:player->no())*(m_height+15), m_width, m_height));
	//m_sprite.setScale(Vector2f(0.9,0.9));
	m_sprite.setPosition(Vector2f(s_mapOffsetX+m_x,s_mapOffsetY + m_y));
	//m_sprite.setTextureRect(sf::IntRect(0, 0,m_width, m_height));
	//m_sprite.setColor(m_color);
	
	if(no!=-1)
		updatePos();
}



Request Warehouse::update()
{
	//create unit
	Request req={NO_REQUEST,0,0,0,0,(unsigned long int)m_player->no()};
	updatePos();
	if(clock()-m_clock>CLOCKS_PER_SEC/10 && m_player->population() < m_player->populationMax())
	{
		m_clock=clock();
		req.type=R_CREATE_UNIT;

		m_test++;
		std::cout << "Warehouse try to create unit" << std::endl;
		Unit unit;
		Element::placeAround(this,&unit,&req.val1,&req.val2);

	}
	return req;
}
