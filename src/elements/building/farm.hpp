#ifndef FARM_H
#define FARM_H

#include "../element.hpp"
#include "../../macro.txt"
#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Farm: public Buildable{

public:
   Farm(): Farm(-1,0,0){};
   Farm(int no, int px, int py, Player * player=NULL): Buildable(no,px,py,player)
   { 
      m_isBuidable = true;
      m_width = s_width;
      m_height = s_height;
      m_HP = s_HP;
      m_defense = s_defense;
      m_type = s_type;
      m_color = s_color;
      m_pathButtonTexture = s_pathButtonTexture;

      m_sprite.setTexture(*s_texture);
      //m_sprite.setScale(Vector2f(0.25,0.25));
      m_sprite.setPosition(Vector2f(s_mapOffsetX+m_x,s_mapOffsetY + m_y));
      //m_sprite.setTextureRect(sf::IntRect(0, 0,m_width, m_height));
      //m_sprite.setColor(m_color);
      
      //to compensate mouse middle effect
      m_x -= m_width/2; 
      m_y -= m_height/2;
      
      updatePos();
   };
   
   ~Farm(){};
   
   Request update(){}; //TODO
   static void setting();
   Element * builder(int pno, int px, int py, Player * player=NULL){return new Farm(pno,px,py,player);};

  static void setTexture()
  {
    Image im;
    if (!im.loadFromFile("media/elements/farm1.jpg"))
    {
      cout << "Erreur chargement image!"<< endl;
      // return
    }
    im.createMaskFromColor(Color::White);
    
    const_cast<Texture*>(s_texture)->loadFromImage(im);
  }


  int findTarget(); 

  private:
  void setSize(int pw,int ph){};
  void setID(int pw,int ph){};

  int m_nbpop; //capacity of population added when build
   
  const static Texture * s_texture;
  const static int s_type = TOWER_TYPE;
  const static int s_width = TOWER_WIDTH;
  const static int s_height = TOWER_HEIGHT;
  const static int s_HP = TOWER_HP;
  const static int s_defense = TOWER_DEFENSE;
  //const static int s_color = TOWER_COLOR;
  const static int s_nbpop = FARM_NBPOP;
  const static char s_pathButtonTexture[];
  const static Color s_color;
};

#endif
