#ifndef ELEMENT_MODEL_HPP
#define ELEMENT_MODEL_HPP

#include <iostream>
#include <string>
#include <vector>

#include "../../macro.txt"
#include "../element.hpp"

using namespace std;



class ElementModel: public Element
{
	public:

	ElementModel(): ElementModel(-1,0,0){};

	ElementModel(int no, int px, int py,  Player * player = NULL);

	~ElementModel();
	
	//load a instance of this class in the elements array of the classElement
	static void setting();
	//usually called thanks to the instance stocked in the element array
	Element * builder(int pno, int px, int py,Player * player=NULL){return new ElementModel(pno,px,py,player);};
	std::string getInfo(){return "ElementModel";};
	
	//called each turn
	Request update();
	
	//called in setting to load each texture
	static void setTexture();

	/*only for moving element
	Request reqMoveToTarget();
	int move(int dx,int dy);
	int attack(){};
	void select();
	void unselect();*/

	
	private:
	void setSize(int pw,int ph){};
	void setID(int pw,int ph){};

	const static Texture * s_texture;
	const static Texture * s_textureTarget;
	const static int s_type = ElementModel_TYPE;
	const static int s_width = ElementModel_WIDTH;
	const static int s_height = ElementModel_HEIGHT;
	const static int s_HP = ElementModel_HP;
	const static int s_defense = ElementModel_DEFENSE;
	const static Color s_color;
};

#endif
