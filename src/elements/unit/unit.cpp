#include "unit.hpp"

const Color Unit::s_color = UNIT_COLOR;

void Unit::setting()
{
	//Unit:setTexture();
	Element::elements()[UNIT_TYPE] = new Unit();
};

Request Unit::update()
{

	Request req={NO_REQUEST,0,0,0,(unsigned long int)this};
	updatePos();
	if(clock()-m_clock>CLOCKS_PER_SEC/8)
	{
		m_clock=clock();
		req.type=R_MOVE;
		req.val1 = 1;
		req.val2 = -1;
	}
return req;
}

int  Unit::move(int dx,int dy)
{
	for(int ie = 0 ; ie < m_height ; ie++)//mark the whole area of the element as 0
		for(int je = 0; je < m_width ; je++)
			s_map[(m_y+ie)*s_mapWidth + m_x + je] = 0;
	if(isPlaceFree(m_x+dx,m_y+dy,m_width,m_height))
	{
		m_x+=dx;
		m_y+=dy;
		updatePos();
	}
	else
		cout << "can't move" << endl;
}
