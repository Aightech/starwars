#include "element.hpp"


unsigned int* Element::s_map = NULL;
int Element::s_mapHeight = 0;
int Element::s_mapWidth = 0;
int Element::s_team = 0;

//store one of each element to get access to them through Element class.
Element* Element::elementsType[]= ELEMENTS_ARRAY;

void Element::updatePos()
{
       updateID();
       cout << "test" <<endl;
       for(int ie = 0 ; ie < m_height ; ie++)//mark the whole area of the element as marked
              for(int je = 0; je < m_width ; je++)
                     s_map[(m_y+ie)*s_mapWidth + m_x + je] = m_ID;
              
}
