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

	Warehouse(int no, int px, int py,  Player * player=NULL);

	~Warehouse(){};
	std::string getInfo(){return "Warehouse";};
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
	const static char s_pathButtonTexture2[];
	const static Color s_color;


};

#endif
