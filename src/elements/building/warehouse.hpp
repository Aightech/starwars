#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "../element.hpp"
#include "../../macro.txt"
#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Warehouse: public Element
{

	public:
	Warehouse(): Warehouse(-1,0,0){};

	Warehouse(int no, int px, int py): Element(no,px,py)
	{
		m_width = s_width;
		m_height = s_height;
		m_HP = s_HP;
		m_defense = s_defense;
		m_type = s_type;
		m_color = s_color;


		//to compensate mouse middle effect
		m_x -= m_width/2; 
		m_y -= m_height/2;
		Texture t;
		m_sprite.setTexture(t);
		m_sprite.setPosition(Vector2f(s_mapOffsetX+m_x,s_mapOffsetY + m_y));
		m_sprite.setTextureRect(sf::IntRect(0, 0,m_width, m_height));
		m_sprite.setColor(m_color);
		
		if(no!=-1)
			updatePos();
	};

	~Warehouse(){};

	Request update();


	private:
	void setSize(int pw,int ph){};
	void setID(int pw,int ph){};

	const static Texture s_texture;
	const static int s_type = WAREHOUSE_TYPE;
	const static int s_width = WAREHOUSE_WIDTH;
	const static int s_height = WAREHOUSE_HEIGHT;
	const static int s_HP = WAREHOUSE_HP;
	const static int s_defense = WAREHOUSE_DEFENSE;
	const static Color s_color;


};

#endif
