#ifndef BUTTONLIB_H
#define BUTTONLIB_H


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <SFML/Config.hpp>
#define GAME_THEME_INFOBOX "media/theme/dialogBox.png"

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
	Button(GUI* pgui,std::string s, sf::Vector2f position,int nb = 0,void (GUI::*f)(int)=NULL);
	Button(GUI* pgui, const std::string &filename, int pw, int ph, sf::Vector2f position, void (GUI::*f)(int),int nb=0,std::string info = "");
	void create(std::string s, sf::Vector2f position,int nb = 0);
	~Button(){s_nb--;};//std::cout<<"deleted"<<std::endl;};
	
	void setString(std::string s, Color color =Color(255,255,255));
	static void setTexture(const std::string &filename, int pw, int ph)
	{s_texture->loadFromFile(filename);s_width=pw;s_height=ph;
	s_infoBox->loadFromFile(GAME_THEME_INFOBOX);};
	static void setFont(const std::string &filename)
	{s_font->loadFromFile(filename);};
	
	void setTexture(const std::string &filename)
	{m_texture->loadFromFile(filename);};

	int update(RenderWindow &w);
	int& clicked(){return m_state;};

	int getNbOfBt(){return s_nb-1;};

	void disable(){m_state=-1;
	#if SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 4
		m_label.setFillColor(Color(155,155,155));
	#else
		m_label.setColor(Color(155,155,155));
	#endif
		m_button.setColor(Color(155,155,155));
	};//m_label.setFillColor(Color(155,155,155));};
	void enable(){m_state=0;
	#if SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 4
		m_label.setFillColor(Color(255,255,255));
	#else
		m_label.setColor(Color(255,255,255));
	#endif
	m_button.setColor(Color(255,255,255));
	};//m_label.setFillColor(Color(255,255,255));};
	
	void setPosition(sf::Vector2f position);
	


	private:
	static int s_nb;
	static Texture* s_texture;
	static Texture* s_infoBox;
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
	Text m_info;
	Sprite m_infoBox;

	int m_state;
	int m_msInButton;
	int m_click;

};



#endif
