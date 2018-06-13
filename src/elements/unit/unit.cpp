#include "unit.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

const Color Unit::s_color = UNIT_COLOR;
const Texture* Unit::s_texture = new Texture();
const Texture* Unit::s_textureTarget = new Texture();



Unit::Unit(int no, int px, int py,  Player * player):Element(no,px,py,player)
{
	m_width = s_width;
	m_height = s_height;
	m_HP = s_HP;
	m_defense = s_defense;
	m_type = s_type;
	m_color = s_color;
	m_speed =1;
	
	setTarget( px, py);
	
	m_targetAnimation.set(s_textureTarget,m_targetX,m_targetY,44,42,14,0.2);

	m_sprite.setTexture(*s_texture);
	//m_sprite.setScale(Vector2f(2,2));
	m_sprite.setPosition(Vector2f(s_mapOffsetX+m_x,s_mapOffsetY + m_y));
	m_sprite.setTextureRect(sf::IntRect(0, ((no==-1)?(unsigned long int)player:player->no())*(m_height), m_width, m_height));
	if(no!=-1)
		updatePos();
	
	if(player!=NULL)
		player->population()++;
}
Unit::~Unit()
{
	if(m_player!=NULL)
		m_player->population()--;
}

void Unit::setting()
{
	Unit:setTexture();
	Element::elements()[UNIT_TYPE] = new Unit();
};

void Unit::setTexture()
{
	Image im;
	if (!im.loadFromFile("media/elements/unit2.png"))
	{
		cout << "Erreur chargement image!"<< endl;
		// return
	}
	//std :: cout << "Image chargée" << std :: endl; 
	//im.createMaskFromColor(Color::White);
	const_cast<Texture*>(s_texture)->loadFromImage(im);
	
	
	
	if (!im.loadFromFile("media/theme/targetPointer.png"))
	{
		cout << "Erreur chargement image!"<< endl;
		// return
	}
	const_cast<Texture*>(s_textureTarget)->loadFromImage(im);
}

void Unit::select()
{
	m_selected = 1;
	m_sprite.setTextureRect(sf::IntRect(m_nbState*m_selected* m_width, m_player->no()*(m_height), m_width, m_height));
}
void Unit::unselect()
{
	m_selected = 0;
	m_sprite.setTextureRect(sf::IntRect(m_nbState*m_selected* m_width, m_player->no()*(m_height), m_width, m_height));
}
Request Unit::update()
{

	Request req={NO_REQUEST,0,0,0};
	req.e =(unsigned long int)m_no;
	updatePos();
	if(clock()-m_clock>CLOCKS_PER_SEC/100)
	{
		m_clock=clock();
		cout<< "try to move" << req.e<< endl;
		req = reqMoveToTarget();
		if(abs(m_targetX-m_x) < 50 && abs(m_targetY-m_y) < 50)
		{
			req.val1 = 0;
			req.val2=0;
		}
		int range = 10;
		unsigned long int ptr = isPlaceOccupied(m_x-range/2 +req.val1, m_y-range/2 +req.val2, m_width + range, m_height + range, this);
		if(ptr != 0 && ptr != -1 && ((Element*)ptr)->player()->no() != m_player->no())
		{
			req.type = R_ATTACK;
			req.e = (unsigned long int)((Element*)ptr)->no();
			req.val1 = 10;
			//((Element*)ptr)->getDamage(10);
		}
		cout<< "end to move"<< endl;
	}
return req;
}

Request Unit::reqMoveToTarget()
{
	Request req={NO_REQUEST,0,0,0,(unsigned long int)this};
	req.type=R_MOVE;
	if(m_dx < m_dy)
	{
		m_over += m_dx;
		if(m_over >= m_dy)
		{
			m_over -= m_dy;
			req.val1 = m_dirx;
		}
		req.val2 = m_diry;
	}
	else
	{
		m_over += m_dy;
		if(m_over >= m_dx)
		{
			m_over -= m_dx;
			req.val2 = m_diry;
		}
		req.val1 = m_dirx;
	}
	//cout << "no: "<< m_no << " x: "<< req.val1 << " y: "<< req.val2 << endl;
	return req;
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
//	else
//		cout << "can't move" << endl;
	updatePos();
	return 0;
}
