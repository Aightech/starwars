#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>
#include <string>
#include <vector>

#include "../macro.txt"

//using namespace sf;
using namespace std;



class Element
{
       public:
       Element(){};
       Element(int no, int px, int py):m_no(no),m_x(px),m_y(py){};
       
       ~Element(){};
       
       //TODO tranform to return a action 
       virtual int update()=0;
       
       int x(){return m_x;};
       int y(){return m_y;};
       int HP(){return m_HP;};
       
       int getDamage(int damage)
       {
              m_HP -= m_defense*damage;
              updatePos();
              return m_HP;
       };
       
       int width(){return m_width;}
       int height(){return m_height;}
       
       
       static unsigned int*& map(){return s_map;};
       static int& mapHeight(){return s_mapHeight;};
       static int& mapWidth(){return s_mapWidth;};
       
       
       static Element* elementsType[NB_ELEMENT];
       
       
       
       protected:
       void updatePos();
       
       //     team numero  HP  type
       // ID:    4| 999 9|99 9|99
       int updateID()
       {return (m_ID = s_team*1000000000 + 100000*m_no + 100*m_HP + m_type);};
       
       
       void setPos(int px, int py){m_x=px;m_y=py;};
       void virtual setSize(int pw,int ph) = 0;
       void virtual setID(int pw,int ph) = 0;
       
       unsigned int m_ID;
       int m_no;
       int m_type;
       int m_color;
       
       static unsigned int * s_map;
       static int s_mapHeight;
       static int s_mapWidth;
       
       
       
       int m_x;
       int m_y;
       int m_width;
       int m_height;
       
       int m_HP;
       float m_defense;
       static int s_team;

       
       
       
};


#include "building/warehouse.hpp"

#endif
