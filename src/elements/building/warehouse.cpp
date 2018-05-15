#include "warehouse.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

const Color Warehouse::s_color = WAREHOUSE_COLOR;
const Texture* Warehouse::s_texture = new Texture();


Request Warehouse::update()
{
	//create unit
	Request req={NO_REQUEST,0,0,0,0,(unsigned long int)this};
	updatePos();
	if(clock()-m_clock>CLOCKS_PER_SEC && m_test < 5)
	{
		m_clock=clock();
		req.type=R_CREATE_UNIT;

		m_test++;

		Unit unit;
		Element::placeAround(this,&unit,&req.val1,&req.val2);

	}
	return req;
}
