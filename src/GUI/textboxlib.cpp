#include "textboxlib.hpp"
#include <stdio.h>

int Textbox::s_nb=0;
Font* Textbox::s_font = new Font();

Textbox::Textbox(std::string *s, sf::Vector2f position,int nb)
{
       //box.setTexture(s_texture);
       //box.setTextureRect(IntRect(0, 0, 137, 50));
       box.setFillColor(sf::Color(255, 255, 255));
       box.setSize(sf::Vector2f(100, 100));
       box.setPosition(position);
       bxPos=box.getPosition();
       bxSize=box.getGlobalBounds();
       m_state=0;
       
       label.setFont(*s_font);
       label.setString(*s);
       label.setCharacterSize(20);
       label.setPosition(Vector2f(bxPos.x+(bxSize.width-s->size()*13)/2, bxPos.y+(bxSize.height-5-20)/2));
       label.setFillColor(Color(255,255,255));
       
       m_nb=nb;
       s_nb++;
       
}

