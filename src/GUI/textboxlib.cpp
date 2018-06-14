#include "textboxlib.hpp"
#include <stdio.h>

int Textbox::s_nb=0;
Font* Textbox::s_font = new Font();

Textbox::Textbox(std::string s, sf::Vector2f position,int nb)
{
       //box.setTexture(s_texture);
       m_box.setTextureRect(IntRect(0, 0, 183, 43));
       m_box.setColor(sf::Color(255, 255, 255));
       //m_box.setSize(sf::Vector2f(100, 100));
       m_box.setPosition(position);
       m_bxPos=m_box.getPosition();
       m_bxSize=m_box.getGlobalBounds();
       m_state=0;
       
       m_label.setFont(*s_font);
       m_label.setString(s);
       m_label.setCharacterSize(20);
       m_label.setPosition(Vector2f(m_bxPos.x+10, m_bxPos.y+(m_bxSize.height-5-20)/2));
       //label.setFillColor(Color(255,255,255));
       
       m_nb=nb;
       s_nb++;
       
}

int Textbox::update(RenderWindow &w)
{
	w.draw(m_box);
	w.draw(m_label);
}

