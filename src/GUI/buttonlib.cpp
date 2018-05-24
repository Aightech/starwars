#include "buttonlib.hpp"
#include <stdio.h>


int Button::s_nb=0;
Texture* Button::s_texture = new Texture();
int Button::s_width=0;
int Button::s_height=0;
Font* Button::s_font = new Font();

Button::Button(std::string s, sf::Vector2f position,int nb)
{
	m_texture = s_texture;
	m_width = s_width;
	m_height = s_height;

	m_button.setTexture(*m_texture);
	m_button.setTextureRect(IntRect(0, 0, m_width, m_height));
	m_button.setPosition(position);
	btPos=m_button.getPosition();
	btSize=m_button.getGlobalBounds();
	m_state=0;

	m_label.setFont(*s_font);
	m_label.setString(s);
	m_label.setCharacterSize(20);
	m_label.setPosition(Vector2f(btPos.x+(btSize.width-s.size()*13)/2, btPos.y+(btSize.height-5-20)/2));
	//m_label.setFillColor(Color(255,255,255));

	m_nb=nb;
	s_nb++;

}



Button::Button(GUI* pgui,const std::string &filename, int pw, int ph, sf::Vector2f position, void (GUI::*f)(int),int nb)
{
	m_gui = pgui;
	m_texture = new Texture();
	m_texture->loadFromFile(filename);
	m_width = pw;
	m_height = ph;
	m_fctCllBck = f;

	m_button.setTexture(*m_texture);
	m_button.setTextureRect(IntRect(0, 0, m_width, m_height));
	m_button.setPosition(position);
	btPos=m_button.getPosition();
	btSize=m_button.getGlobalBounds();
	m_state=0;

	m_nb=nb;
	s_nb++;

}

void Button::create(std::string s, sf::Vector2f position,int nb)
{
	m_texture = s_texture;
	m_width = s_width;
	m_height = s_height;

	m_button.setTexture(*m_texture);
	m_button.setTextureRect(IntRect(0, 0, m_width, m_height));
	m_button.setPosition(position);
	btPos=m_button.getPosition();
	btSize=m_button.getGlobalBounds();
	m_state=0;

	m_label.setFont(*s_font);
	m_label.setString(s);
	m_label.setCharacterSize(20);
	m_label.setPosition(Vector2f(btPos.x+(btSize.width-s.size()*13)/2, btPos.y+(btSize.height-5-20)/2));
	//m_label.setFillColor(Color(255,255,255));

	m_nb=nb;
}

int Button::update(RenderWindow &w)
{            
//       
       msPos=Mouse::getPosition(w);
       if(msPos.x >= btPos.x
       && msPos.x <= btPos.x + btSize.width
       && msPos.y >= btPos.y
       && msPos.y <= btPos.y + btSize.height)
       {
              m_msInButton=1;
              if(m_state<1)
                     m_button.setTextureRect(IntRect(m_width, 0, m_width, m_height));
       }
       else
       {
              m_msInButton=0;
              if(m_state<1)
                     m_button.setTextureRect(IntRect(0, 0, m_width, m_height));
       }
       
       if(m_msInButton==1 && Mouse::isButtonPressed(Mouse::Left) && m_click==0)
       {
              m_click=1;
              if(m_state==-1) return 0;
              if(m_state==0)
              {
                     m_state=1;
                     m_button.setTextureRect(IntRect(2*m_width, 0, m_width, m_height));
              }
              else
              {
                     m_state=0;
                     m_button.setTextureRect(IntRect(0, 0, m_width, m_height));
              }
              (*m_gui.*m_fctCllBck)(m_nb);
              return m_nb;
       }
       if(!Mouse::isButtonPressed(Mouse::Left))//to ensure the click has been released
             m_click=0; 
             
       w.draw(m_button);
       w.draw(m_label);
       return 0;
}

void Button::setPosition(sf::Vector2f position)
{
	m_button.setPosition(position);
	btPos=m_button.getPosition();
	btSize=m_button.getGlobalBounds();

	std::string s = m_label.getString();
	m_label.setPosition(Vector2f(btPos.x+(btSize.width-s.size()*13)/2, btPos.y+(btSize.height-5-20)/2));
}
