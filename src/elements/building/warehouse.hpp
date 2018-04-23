#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "../element.hpp"
#include "../../macro.txt"
#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Warehouse: public Element
{

       public:
       Warehouse()
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
              //updatePos();
       };
       Warehouse(int no, int px, int py): Element(no,px,py)
       { 
              
              m_width = s_width;
              m_height = s_height;
              m_HP = s_HP;
              m_defense = s_defense;
              m_type = s_type;
              m_color = s_color;
              
              //to compensate mouse middle effect
              m_x -= m_width/2; 
              m_y -= m_height/2;
              
              
              updatePos();
       };
       
       ~Warehouse(){};
       
       int update();
       
       
       private:
       void setSize(int pw,int ph){};
       void setID(int pw,int ph){};
       
       const static int s_type = WAREHOUSE_TYPE;
       const static int s_width = WAREHOUSE_WIDTH;
       const static int s_height = WAREHOUSE_HEIGHT;
       const static int s_HP = WAREHOUSE_HP;
       const static int s_defense = WAREHOUSE_DEFENSE;
       const static int s_color = WAREHOUSE_COLOR;
       
       
};

#endif
