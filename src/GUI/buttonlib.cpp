#include "buttonlib.hpp"
#include <stdio.h>


int Button::s_nb=0;
Texture* Button::s_texture = new Texture();
Font* Button::s_font = new Font();

Button::Button(std::string s, sf::Vector2f position,int nb)
{
       button.setTexture(*s_texture);
       button.setTextureRect(IntRect(0, 0, 137, 50));
       button.setPosition(position);
       btPos=button.getPosition();
       btSize=button.getGlobalBounds();
       m_state=0;
       
       label.setFont(*s_font);
       label.setString(s);
       label.setCharacterSize(20);
       label.setPosition(Vector2f(btPos.x+(btSize.width-s.size()*13)/2, btPos.y+(btSize.height-5-20)/2));
       label.setColor(Color(255,255,255));
       
       m_nb=nb;
       s_nb++;
       
}


void Button::create(std::string s, sf::Vector2f position,int nb)
{
       button.setTexture(*s_texture);
       button.setTextureRect(IntRect(0, 0, 137, 50));
       button.setPosition(position);
       btPos=button.getPosition();
       btSize=button.getGlobalBounds();
       m_state=0;
       
       label.setFont(*s_font);
       label.setString(s);
       label.setCharacterSize(20);
       label.setPosition(Vector2f(btPos.x+(btSize.width-s.size()*13)/2, btPos.y+(btSize.height-5-20)/2));
       label.setColor(Color(255,255,255));
       
       m_nb=nb;
}

int Button::update(RenderWindow &w)
{            
       
       msPos=Mouse::getPosition(w);
       if(msPos.x >= btPos.x
       && msPos.x <= btPos.x + btSize.width
       && msPos.y >= btPos.y
       && msPos.y <= btPos.y + btSize.height)
       {
              msInButton=1;
              if(m_state<1)
                     button.setTextureRect(IntRect(150, 0, 137, 50));
       }
       else
       {
              msInButton=0;
              if(m_state<1)
                     button.setTextureRect(IntRect(0, 0, 137, 50));
       }
       
       if(msInButton==1 && Mouse::isButtonPressed(Mouse::Left) && click==0)
       {
              click=1;
              if(m_state==-1) return 0;
              if(m_state==0)
              {
                     m_state=1;
                     button.setTextureRect(IntRect(301, 0, 137, 50));
              }
              else
              {
                     m_state=0;
                     button.setTextureRect(IntRect(0, 0, 137, 50));
              }
              return m_nb;
       }
       if(!Mouse::isButtonPressed(Mouse::Left))//to ensure the click has been released
             click=0; 
             
       w.draw(button);
       w.draw(label);
       return 0;
}
