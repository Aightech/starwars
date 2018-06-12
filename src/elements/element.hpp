#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "../macro.txt"
#include "../GUI/animlib.hpp"


using namespace sf;
using namespace std;

class Element;
class Player;

typedef struct _Request {
	int type;
	int val1;
	int val2;
	int val3;
	unsigned long int e;
	unsigned long int p;
}Request;



class Element
{
	public:
	Element(){};
	Element(int no, int px, int py, Player * player =NULL):m_no(no),m_x(px),m_y(py),m_player(player){ m_clock = clock();};

	virtual ~Element();

	//TODO tranform to return a action 
	virtual Request update()=0;
	//virtual void setting() = 0;
	virtual Element* builder(int pno, int px, int py, Player * player=NULL)=0;

	int& x(){return m_x;};
	int& y(){return m_y;};
	int no(){return m_no;};
	int& HP(){return m_HP;};
	int type(){return m_type;}
	Player * player(){ return m_player;};
	int isBuidable(){return m_isBuidable;};
	Sprite& sprite(){return m_sprite;};
	Animation& targetAnimation(){return m_targetAnimation;};
	Color& color(){return m_color;};
	virtual void select(){};
	virtual void unselect(){};
	bool isSelected(){ return m_selected;};
	
	virtual std::string getInfo(){};


	int getDamage(int damage)
	{
		m_HP -= damage/m_defense;
		updatePos();
		return m_HP;
	};

	int width(){return m_width;}
	int height(){return m_height;}
	
	int setTarget(int px,int py);


	static unsigned long int*& map(){return s_map;};
	static int& mapHeight(){return s_mapHeight;};
	static int& mapWidth(){return s_mapWidth;};
	static int& mapOffsetY(){return s_mapOffsetY;};
	static int& mapOffsetX(){return s_mapOffsetX;};

	static unsigned long int isPlaceOccupied(int px,int py,int pw, int ph, Element * elmt =NULL);
	static bool placeAround(Element *fixed, Element *toPlaced,int *x, int *y);
	void updateStatut(int x, int y, int hp);
	
	static Element ** elements();
	static Element * factory(int elementType,int pno, int px, int py, Player * player =NULL)
	{return (s_elements[elementType]!=NULL)?s_elements[elementType]->builder(pno, px, py, player):NULL;};

	protected:
	void updatePos();
	
	static Element ** s_elements;
	//     team numero  HP  type
	// ID:    4| 999 9|99 9|99
	int updateID(){return 1;};
	//{return (m_ID = s_team*1000000000 + 100000*m_no + 100*m_HP + m_type);};


	void setPos(int px, int py){m_x=px;m_y=py;};
	void virtual setSize(int pw,int ph) = 0;
	void virtual setID(int pw,int ph) = 0;

	unsigned int m_ID;
	int m_no;
	int m_type;
	Color m_color;

	static unsigned long int * s_map;

	static int s_mapHeight;
	static int s_mapWidth;
	static int s_mapOffsetY;
	static int s_mapOffsetX;

	Sprite m_sprite;
	
	bool m_isBuidable=false;

	int m_x;
	int m_y;
	int m_width;
	int m_height;
	int m_speed;
	
	Animation m_targetAnimation;
	
	int m_targetX;
	int m_targetY;
	int m_dx;
	int m_dy;
	int m_dirx;
	int m_diry;
	double m_over;
	
	int m_HP;
	float m_defense;
	Player *m_player;
	
	int m_selected=0;
	int m_nbState=3;

	clock_t m_clock;
	int m_test =0;


};

class Buildable: public Element
{
	public:
	Buildable(int no, int px, int py, Player * player=NULL): Element(no,px,py,player){};
	const char * getButtonTexture(int i=0){	return (i==0)?m_pathButtonTexture:m_pathButtonTexture2;	};
	
	protected:
	
	const char * m_pathButtonTexture;
	const char * m_pathButtonTexture2;
};


#include "../player/playerlib.hpp"
#include "building/warehouse.hpp"
#include "building/tower.hpp"
#include "building/farm.hpp"

#include "building/headquarter.hpp"

#include "unit/unit.hpp"

#endif
