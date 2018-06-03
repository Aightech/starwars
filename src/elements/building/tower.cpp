#include "tower.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

const Texture* Tower::s_texture = new Texture();
const char Tower::s_pathButtonTexture[] = TOWER_PATH_BUTTON_TEXTURE;
const char Tower::s_pathButtonTexture2[] = TOWER_PATH_BUTTON_TEXTURE2;
const Color Tower::s_color = TOWER_COLOR;

/*int Tower::findTarget(){
//find a reachable target around the tower and return its id, return -1 if no one is reachable
	for (int i=0; i<m_reach; i++){ 
		for (int j=0; j<m_reach; j++){
			if( ((Element *) s_map[(m_y+i)*s_mapWidth + m_x + j])->type() == UNIT_TYPE){ //TODO  SUPER_UNIT_TYPE 
				return ((Element *) s_map[(m_y+i)*s_mapWidth + m_x + j])->id(); //first unit find in the area is returned
			}
		}	
	}  
	return -1;

}*/

void Tower::setting()
{
       Tower::setTexture();
       Element::elements()[TOWER_TYPE] = new Tower();
};

Tower::Tower(int no, int px, int py, Player * player): Buildable(no,px,py,player)
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
          m_sprite.setTextureRect(sf::IntRect(0, ((no==-1)?(unsigned long int)player:player->no())*(m_height), m_width, m_height));
          //m_sprite.setColor(m_color);
          
          //to compensate mouse middle effect
//          m_x -= m_width/2; 
//          m_y -= m_height/2;
          
          updatePos();
  }

Request Tower::update()
{
       //create unit
       Request req={-1,0,0};
       updatePos();
       if(clock()-m_clock>CLOCKS_PER_SEC && m_test <5)
       {
              m_clock=clock();
              req.type=R_ATTACK;
              m_test++;
              
              //Tower::findTarget(this,&req.val1,&req.val2);
            
              
       }
       return req;
}
