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

	Unit(int no, int px, int py,  Player * player = NULL);

	~Unit(){};
	int moveTo(int x, int y);
	int move(int dx,int dy);
	int attack(){};

	static void setting();
	Request update();
	Element * builder(int pno, int px, int py,Player * player=NULL){return new Unit(pno,px,py,player);};
	std::string getInfo(){return "Unit";};

	static void setTexture()
	{
		Image im;
		if (!im.loadFromFile("media/elements/bb81.png"))
		{
			cout << "Erreur chargement image!"<< endl;
			// return
		}
		std :: cout << "Image chargée" << std :: endl; 
		im.createMaskFromColor(Color::White);
		
		const_cast<Texture*>(s_texture)->loadFromImage(im);
	}

	private:
	void setSize(int pw,int ph){};
	void setID(int pw,int ph){};

	const static Texture * s_texture;
	const static int s_type = UNIT_TYPE;
	const static int s_width = UNIT_WIDTH;
	const static int s_height = UNIT_HEIGHT;
	const static int s_HP = UNIT_HP;
	const static int s_defense = UNIT_DEFENSE;
	const static Color s_color;
};

#endif
