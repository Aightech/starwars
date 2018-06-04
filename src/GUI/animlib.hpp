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
	Animation(){};
	Animation(std::string s, int px, int py, int width=-1, int height=-1, int nbFrame=0, double frameRate = 1);
	Animation(Texture* texture, int px, int py, int width=-1, int height=-1, int nbFrame=0, double frameRate = 1);
	
	void set(const Texture* texture, int px, int py, int width=-1, int height=-1, int nbFrame=0, double frameRate = 1);
	
	~Animation(){if(m_ownTexture)delete m_texture;};
	void setPosition(sf::Vector2f position);
	void setScale(float sx, float sy);
	int update(RenderWindow &w);

	private:
	bool m_ownTexture=false;
	const Texture* m_texture;
	Sprite m_sprite;
	int m_x;//x pos of the animation
	int m_y;//y pos of the animation
	int m_width; //width of one picture of the animation
	int m_height; //height of one picture of the animation
	int m_nbFrame; //total number of images to display
	int m_state; //number of the current displayed image
	
	clock_t m_clock;//to ensure the frame rate
	double m_frameRate;
};

#endif
