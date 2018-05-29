#include "unit.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

const Color Unit::s_color = UNIT_COLOR;
const Texture* Unit::s_texture = new Texture();



Unit::Unit(int no, int px, int py,  Player * player): Element(no,px,py,player)
{
	m_width = s_width;
	m_height = s_height;
	m_HP = s_HP;
	m_defense = s_defense;
	m_type = s_type;
	m_color = s_color;
	m_speed =1;
	
	m_targetX = 0;
	m_targetY = s_mapWidth;

	m_sprite.setTexture(*s_texture);
	//m_sprite.setScale(Vector2f(2,2));
	m_sprite.setPosition(Vector2f(s_mapOffsetX+m_x,s_mapOffsetY + m_y));
	//m_sprite.setTextureRect(sf::IntRect(0, 0,m_width, m_height));
	//m_sprite.setColor(m_color);
	if(no!=-1)
		updatePos();
	
	if(player!=NULL)
		player->population()++;
	std :: cout << "Unit created" << std :: endl; 
}

void Unit::setting()
{
	Unit:setTexture();
	Element::elements()[UNIT_TYPE] = new Unit();
};

Request Unit::update()
{

	Request req={NO_REQUEST,0,0,0,(unsigned long int)this};
	updatePos();
	if(clock()-m_clock>CLOCKS_PER_SEC/100)
	{
		m_clock=clock();
		int dx = m_targetX - m_x;
		int dy = m_targetY - m_y;
		
		int dirx=dx>0?m_speed:-m_speed;
		int diry=dy>0?m_speed:-m_speed;
		dx=abs(dx);
		dy=abs(dy);
		//over=dx/2;
		//unsigned long int ptr = isPlaceOccupied(m_x+dx,m_y+dy,m_width,m_height));
		if(1)//ptr == 0)
		{
			req.type=R_MOVE;
			req.val1 = (m_player->no()==0)?1:-1;
			req.val2 = -1;
		}
		else
		{
		
		}
	}
return req;
}

int Unit::moveTo(int x, int y)
{
	float dx = x-m_x;
	float dy = y-m_y;
	
	
}

int  Unit::move(int dx,int dy)
{
	for(int ie = 0 ; ie < m_height ; ie++)//mark the whole area of the element as 0
		for(int je = 0; je < m_width ; je++)
			s_map[(m_y+ie)*s_mapWidth + m_x + je] = 0;
	if(!isPlaceOccupied(m_x+dx,m_y+dy,m_width,m_height))
	{
		m_x+=dx;
		m_y+=dy;
		updatePos();
		return 1;
	}
	else
		cout << "can't move" << endl;
	updatePos();
	return 0;
}
