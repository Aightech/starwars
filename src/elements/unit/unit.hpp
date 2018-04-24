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
       
       Unit()
       {
              m_width = s_width;
              m_height = s_height;
              m_HP = s_HP;
              m_defense = s_defense;
              m_type = s_type;
              m_color = s_color;
              
              //to compensate mouse middle effect
              m_x =0;
              m_y =0;
       };
       Unit(int no, int px, int py): Element(no,px,py)
       {
              m_width = s_width;
              m_height = s_height;
              m_HP = s_HP;
              m_defense = s_defense;
              m_type = s_type;
              m_color = s_color;
              
              
              updatePos();
       };
       
       ~Unit(){};
       
       int move(int dx,int dy);
       int attack(){};
       
       
       Request update();
       
       private:
       void setSize(int pw,int ph){};
       void setID(int pw,int ph){};
       
       const static int s_type = UNIT_TYPE;
       const static int s_width = UNIT_WIDTH;
       const static int s_height = UNIT_HEIGHT;
       const static int s_HP = UNIT_HP;
       const static int s_defense = UNIT_DEFENSE;
       const static int s_color = UNIT_COLOR;
};

#endif
