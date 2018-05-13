#ifndef BUTTONLIB_H
#define BUTTONLIB_H


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

using namespace sf;


class Button
{
	/*! \class Game
	* \brief This class represent the game application.
	*      It owns the different methods running the game.
	*/
	public:
	Button(){s_nb++;};
	Button(std::string s, sf::Vector2f position,int nb = 0);
	void create(std::string s, sf::Vector2f position,int nb = 0);
	~Button(){s_nb--;};//std::cout<<"deleted"<<std::endl;};

	static void setTexture(const std::string &filename)
	{s_texture->loadFromFile(filename);};
	static void setFont(const std::string &filename)
	{s_font->loadFromFile(filename);};

	int update(RenderWindow &w);
	int clicked(){return m_state;};

	int getNbOfBt(){return s_nb-1;};

	void disable(){m_state=-1;label.setColor(Color(155,155,155));};
	void enable(){m_state=0;label.setColor(Color(255,255,255));};


	private:
	static int s_nb;
	static Texture* s_texture;
	static Font* s_font;
	Vector2i msPos;
	Vector2f btPos;
	FloatRect btSize;

	int m_nb;

	Text label;
	Texture* m_texture;
	Sprite button;
	

	int m_state;
	int msInButton;
	int click;

};



#endif
