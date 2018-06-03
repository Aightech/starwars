#ifndef FARM_H
#define FARM_H

#include "../element.hpp"
#include "../../macro.txt"
#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Farm: public Buildable{

	public:
	Farm(): Farm(-1,0,0){};
	Farm(int no, int px, int py, Player * player=NULL);

	~Farm(){};

	Request update(){}; //TODO
	static void setting();
	Element * builder(int pno, int px, int py, Player * player=NULL){	return new Farm(pno,px,py,player);};

	std::string getInfo(){	return "Farm";};
	static void setTexture();


	int findTarget(); 

	private:
	void setSize(int pw,int ph){};
	void setID(int pw,int ph){};

	int m_nbpop; //capacity of population added when build

	const static Texture * s_texture;
	const static int s_type = FARM_TYPE;
	const static int s_width = FARM_WIDTH;
	const static int s_height = FARM_HEIGHT;
	const static int s_HP = FARM_HP;
	const static int s_defense = FARM_DEFENSE;
	//const static int s_color = TOWER_COLOR;
	const static int s_nbpop = FARM_NBPOP;
	const static char s_pathButtonTexture[];
	const static char s_pathButtonTexture2[];
	const static Color s_color;
};

#endif
