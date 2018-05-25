#ifndef TOWER_H
#define TOWER_H

#include "../element.hpp"
#include "../../macro.txt"
#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Tower: public Buildable{

public:
   Tower(): Tower(-1,0,0){};
   Tower(int no, int px, int py, Player * player=NULL): Buildable(no,px,py,player)
   { 
          m_isBuidable = true;
          m_width = s_width;
          m_height = s_height;
          m_HP = s_HP;
          m_defense = s_defense;
          m_type = s_type;
          m_color = s_color;
          m_reach = s_reach;
          m_damages = s_damages;
          m_pathButtonTexture = s_pathButtonTexture;
          m_pathButtonTexture2 = s_pathButtonTexture2;

          m_sprite.setTexture(*s_texture);
          //m_sprite.setScale(Vector2f(0.25,0.25));
          m_sprite.setPosition(Vector2f(s_mapOffsetX+m_x,s_mapOffsetY + m_y));
          m_sprite.setTextureRect(sf::IntRect(0, 0,m_width, m_height));
          //m_sprite.setColor(m_color);
          
          //to compensate mouse middle effect
//          m_x -= m_width/2; 
//          m_y -= m_height/2;
          
          updatePos();
   };
   
   ~Tower(){};
   
  Request update();
  static void setting();
  Element * builder(int pno, int px, int py, Player * player=NULL){return new Tower(pno,px,py,player);};

  static void setTexture()
  {
    Image im;
    if (!im.loadFromFile("media/elements/tower1.jpg"))
    {
      cout << "Erreur chargement image!"<< endl;
      // return
    }
    im.createMaskFromColor(Color::White);
    
    const_cast<Texture*>(s_texture)->loadFromImage(im);
  }
	std::string getInfo(){return "Tower";};
   int findTarget(){
    return 0;
   }
  
   private:
   void setSize(int pw,int ph){};
   void setID(int pw,int ph){};

   int m_reach;
   int m_damages;
   
   const static Texture * s_texture;
   const static int s_type = TOWER_TYPE;
   const static int s_width = TOWER_WIDTH;
   const static int s_height = TOWER_HEIGHT;
   const static int s_HP = TOWER_HP;
   const static int s_defense = TOWER_DEFENSE;
   //const static int s_color = TOWER_COLOR;
   const static int s_reach = TOWER_REACH;
   const static int s_damages = TOWER_DAMAGES;
    const static char s_pathButtonTexture[];
    const static char s_pathButtonTexture2[];
    const static Color s_color;
};

#endif
