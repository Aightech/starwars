#ifndef UNIT_HPP
#define UNIT_HPP

#include <iostream>
#include <string>
#include <vector>

#include "../../macro.txt"
#include "../element.hpp"

using namespace std;



class Unit: public Element
{
	public:

	Unit(): Unit(-1,0,0){};

	Unit(int no, int px, int py,  Player * player = NULL): Element(no,px,py,player)
	{
		m_width = s_width;
		m_height = s_height;
		m_HP = s_HP;
		m_defense = s_defense;
		m_type = s_type;
		m_color = s_color;

		Texture t;
		m_sprite.setTexture(t);
		m_sprite.setPosition(Vector2f(s_mapOffsetX+m_x,s_mapOffsetY + m_y));
		m_sprite.setTextureRect(sf::IntRect(0, 0,m_width, m_height));
		m_sprite.setColor(m_color);
		if(no!=-1)
			updatePos();
	};

	~Unit(){};

	int move(int dx,int dy);
	int attack(){};

	static void setting();
	Request update();
	Element * builder(int pno, int px, int py,Player * player=NULL){return new Unit(pno,px,py,player);};

	private:
	void setSize(int pw,int ph){};
	void setID(int pw,int ph){};

	const static int s_type = UNIT_TYPE;
	const static int s_width = UNIT_WIDTH;
	const static int s_height = UNIT_HEIGHT;
	const static int s_HP = UNIT_HP;
	const static int s_defense = UNIT_DEFENSE;
	const static Color s_color;
};

#endif
