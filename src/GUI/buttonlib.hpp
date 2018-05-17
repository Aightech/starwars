#ifndef BUTTONLIB_H
#define BUTTONLIB_H


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

class GUI;

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
	Button(GUI* pgui, const std::string &filename, int pw, int ph, sf::Vector2f position, void (GUI::*f)(int),int nb=0);
	void create(std::string s, sf::Vector2f position,int nb = 0);
	~Button(){s_nb--;};//std::cout<<"deleted"<<std::endl;};
	
	static void setTexture(const std::string &filename, int pw, int ph)
	{s_texture->loadFromFile(filename);s_width=pw;s_height=ph;};
	static void setFont(const std::string &filename)
	{s_font->loadFromFile(filename);};
	
	void setTexture(const std::string &filename)
	{m_texture->loadFromFile(filename);};

	int update(RenderWindow &w);
	int& clicked(){return m_state;};

	int getNbOfBt(){return s_nb-1;};

	void disable(){m_state=-1;m_label.setColor(Color(155,155,155));};
	void enable(){m_state=0;m_label.setColor(Color(255,255,255));};


	private:
	static int s_nb;
	static Texture* s_texture;
	static int s_width;
	static int s_height;
	static Font* s_font;
	
	void (GUI::*m_fctCllBck)(int);
	
	Vector2i msPos;
	Vector2f btPos;
	FloatRect btSize;

	int m_nb;
	int m_width;
	int m_height;
	GUI * m_gui;

	Text m_label;
	Texture* m_texture;
	Sprite m_button;
	

	int m_state;
	int m_msInButton;
	int m_click;

};



#endif
