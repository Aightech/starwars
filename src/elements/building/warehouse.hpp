#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "../element.hpp"
#include "../../macro.txt"
#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Warehouse: public Buildable
{

	public:
	Warehouse(): Warehouse(-1,0,0){};

	Warehouse(int no, int px, int py,  Player * player=NULL): Buildable(no,px,py,player)
	{
		m_isBuidable = true;
		m_width = s_width;
		m_height = s_height;
		m_HP = s_HP;
		m_defense = s_defense;
		m_type = s_type;
		m_color = s_color;
		m_pathButtonTexture = s_pathButtonTexture;

		m_sprite.setTexture(*s_texture);
		m_sprite.setScale(Vector2f(0.25,0.25));
		m_sprite.setPosition(Vector2f(s_mapOffsetX+m_x,s_mapOffsetY + m_y));
		//m_sprite.setTextureRect(sf::IntRect(0, 0,m_width, m_height));
		//m_sprite.setColor(m_color);
		
		if(no!=-1)
			updatePos();
	};

	~Warehouse(){};

	Request update();
	static void setting();
	Element * builder(int pno, int px, int py, Player * player=NULL){return new Warehouse(pno,px,py,player);};
	
	
	static void setTexture()
	{
		Image im;
		if (!im.loadFromFile("media/elements/warehouse.jpg"))
		{
			cout << "Erreur chargement image!"<< endl;
			// return
		}
		im.createMaskFromColor(Color::White);
		
		const_cast<Texture*>(s_texture)->loadFromImage(im);
	}
	
	const static int width(){return s_width;}
	const static int height(){return s_height;}
	
	

	private:
	void setSize(int pw,int ph){};
	void setID(int pw,int ph){};

	const static Texture * s_texture;
	const static int s_type = WAREHOUSE_TYPE;
	const static int s_width = WAREHOUSE_WIDTH;
	const static int s_height = WAREHOUSE_HEIGHT;
	const static int s_HP = WAREHOUSE_HP;
	const static int s_defense = WAREHOUSE_DEFENSE;
	const static char s_pathButtonTexture[];
	const static Color s_color;


};

#endif
