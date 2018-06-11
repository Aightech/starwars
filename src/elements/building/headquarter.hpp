#ifndef HEADQUARTER_H
#define HEADQUARTER_H

#include "../element.hpp"
#include "../../macro.txt"
#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Headquarter: public Buildable
{

	public:
	Headquarter(): Headquarter(-1,0,0){};

	Headquarter(int no, int px, int py,  Player * player=NULL);

	~Headquarter(){};
	std::string getInfo(){return "Headquarter";};
	Request update();
	static void setting();
	Element * builder(int pno, int px, int py, Player * player=NULL){return new Headquarter(pno,px,py,player);};
	
	
	static void setTexture()
	{
		Image im;
		if (!im.loadFromFile("media/elements/headquarter.png"))
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
	const static int s_type = HEADQUARTER_TYPE;
	const static int s_width = HEADQUARTER_WIDTH;
	const static int s_height = HEADQUARTER_HEIGHT;
	const static int s_HP = HEADQUARTER_HP;
	const static int s_defense = HEADQUARTER_DEFENSE;
	const static Color s_color;


};

#endif
