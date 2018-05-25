#include "warehouse.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

const Color Warehouse::s_color = WAREHOUSE_COLOR;
const Texture* Warehouse::s_texture = new Texture();
const char Warehouse::s_pathButtonTexture[] = WAREHOUSE_PATH_BUTTON_TEXTURE;
const char Warehouse::s_pathButtonTexture2[] = WAREHOUSE_PATH_BUTTON_TEXTURE2;

void Warehouse::setting()
{
	Warehouse::setTexture();
	Element::elements()[WAREHOUSE_TYPE] = new Warehouse();
};


Request Warehouse::update()
{
	//create unit
	Request req={NO_REQUEST,0,0,0,0,(unsigned long int)m_player->no()};
	updatePos();
	if(clock()-m_clock>CLOCKS_PER_SEC/10 && m_player->population() < m_player->populationMax())
	{
		m_clock=clock();
		req.type=R_CREATE_UNIT;

		m_test++;
		std::cout << "Warehouse try to create unit" << std::endl;
		Unit unit;
		Element::placeAround(this,&unit,&req.val1,&req.val2);

	}
	return req;
}
