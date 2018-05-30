#ifndef ANIMLIB_H
#define ANIMLIB_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <SFML/Config.hpp>

class GUI;


using namespace sf;


//To create animations with images contained in one same file, verticaly aligned
class Animation
{
	public:
	Animation(const std::string s, const int nb);
	~Animation();
	void setPosition(sf::Vector2f position);

	private:
	Texture* m_texture; 
	const int m_width; //width of one picture of the animation
	const int m_total_n; //total number of images to display
	int m_state; //number of the current displayed image

}

#endif