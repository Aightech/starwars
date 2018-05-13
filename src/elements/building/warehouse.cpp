#include "warehouse.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

const Color Warehouse::s_color = WAREHOUSE_COLOR;

Request Warehouse::update()
{
	//create unit
	Request req={-1,0,0};
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
