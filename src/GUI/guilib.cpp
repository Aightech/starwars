#include "guilib.hpp"
#include "../gamelib.hpp"

#include <iostream>
#include <string>
#include <vector>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

GUI::GUI(unsigned long int * map, int width, int height)
{
       
       //////------- Loading different fonts ------- /////
       Button::setFont(GAME_FONT_BUTTON);
       Button::setTexture(GAME_THEME_BUTTON,137,50);
       
       //////------- WINDOW SETTING ------- /////
       window.create(VideoMode(WIN_W,WIN_H), GAME_NAME); 
       
       //////------- MAP SETTING ------- /////
       m_map = map;//get and share the game map
       m_mapHeight = height;
       m_mapWidth = width;
       m_mapPosX = window.getSize().x-m_mapWidth - 10;//set the position of the map on the window
       m_mapPosY = 10;
       /////--------ELEMENT SETING --------/////
       Element::mapOffsetY()=m_mapPosY;
       Element::mapOffsetX()=m_mapPosX;
       
       
       
       //////------- FLAGS SETTING ------- /////
       m_state=0;
       m_elementSelectedType=0;
       m_mapDraw = new unsigned char[m_mapHeight*m_mapWidth] ();//used in drawMap
       m_mapdrawVal = 0;
       
 

}

//this methode is launch in a thread
int GUI::start(void *pgame)
{       
	//get the game object
	Game * game = (Game *) pgame;

	//create the first GUI context of the app
	createContext();

	Event event;
	/////------- GUI LOOP ------- /////
	while(window.isOpen() && m_state!=GAME_ONLINE)
	{
		/////------- MONITOR EVENTS ------- /////
		while(window.pollEvent(event))
		{
			/////------- WINDOW CLOSED ------- /////
			if(event.type==Event::Closed)
				window.close();
			/////------- WINDOW RESIZED ------- /////
			if (event.type == sf::Event::Resized)
			{
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
				createContext();
			}
			/////------- MOUSE EVENT ------- /////
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{

				if(m_elementSelectedType!=0 && m_elementOk)
				{
					cout << "CLICKED" << endl;
					Request r={m_elementSelectedType,msPos.x-m_mapPosX-Warehouse::width()/2,msPos.y-m_mapPosY-Warehouse::height()/2};
					r.p=-1;
					game->request(&r);
					delete m_elementSelected;
					m_elementSelected = NULL;

					m_elementOk=false;
					m_elementSelectedType=0;
					
					for(int i=0;i<m_arrayButton.size();i++)
						m_arrayButton[i]->clicked()=false;
				
				}



			}
			/////------- KEYBOARD EVENT ------- /////
			if (event.type == sf::Event::TextEntered)
			{
				//if (event.text.unicode < 128)
				char c=event.text.unicode;
				std::cout << "ASCII character typed: " << (int)c << std::endl;
				if(c=='p')
				{
					std::cout << "Attack " << std::endl;
					game->test();
				}
			}

		}

		/////-------  GUI UPDATE ------- /////

		//get the mouse position
		msPos = Mouse::getPosition(window);

		//clear the window
		window.clear(Color(48,48,48));


		/////------- DRAWING GUI OBJECT ARRAYS ------- /////
		for(int i=0;i<m_arraySprite.size();i++)
			window.draw(*m_arraySprite[i]);
		for(int i=0;i<m_arrayText.size();i++)
			window.draw(*m_arrayText[i]);

		/////------- DRAWING MAP ------- /////
		drawMap();
		drawSelection();

		/////------- GET BUTTON'S STATES ------- /////
		int select=0,s=0;
		for(int i=0;i<m_arrayButton.size();i++)
			select+=m_arrayButton[i]->update(window);
		if(select)//if a button was selected
		{
			switch(abs(select))
			{
				case PLAY_BUTT:
					break;
				case LAN_BUTT:
					break;


				case JOIN_S1_BUTT:
				case JOIN_S2_BUTT:
				case JOIN_S3_BUTT:
				case JOIN_S4_BUTT:
				{int server=abs(select)-JOIN_S1_BUTT;}
				break;

				case HOST_BUTT:
				break;

				case KILL_P1_BUTT: 
				case KILL_P2_BUTT: 
				case KILL_P3_BUTT: 
				case KILL_P4_BUTT:            
				break;

				case START_BUTT:           
				break;


				case OPTION_BUTT:
					m_state=OPTION_MENU;
					createContext();
				break;
				case QUIT_BUTT:
					window.close();
				break;

				case BACK_BUTT:
					m_state=MAIN_MENU;
					createContext();
				break;

				case SAVE_BUTT:
				break;
			}
		}

		window.display();
	}

	game->endGUI();
	return 0;
}

void GUI::buildElement(int type)
{
	if(m_elementSelected!=NULL)
		delete m_elementSelected;
	m_elementSelected = Element::factory(type, -1, msPos.x-50/2, msPos.y-50/2);//new Warehouse(-1,msPos.x-50/2,msPos.y-50/2);
	m_elementSelectedType = type;
}

void GUI::createContext()
{
	/////------- FLUSH ALL THE GUI ARRAYS ------- /////
	for(int i=m_arrayButton.size()-1;i>-1;i--)
	{
		delete m_arrayButton[i];
		m_arrayButton.pop_back();
	}
	for(int i=m_arraySprite.size()-1;i>-1;i--)
	{
		delete m_arraySprite[i];
		m_arraySprite.pop_back();
	}
	for(int i=m_arrayText.size()-1;i>-1;i--)
	{
		delete m_arrayText[i];
		m_arrayText.pop_back();
	}

	/////------- CREATION F CONTEXTS ------- /////
	switch(m_state)
	{
		case MAIN_MENU:
		{
			string buttonsLabel[]={"play","lan","options","quit"};
			int buttonIndex[]={PLAY_BUTT,LAN_BUTT,OPTION_BUTT,QUIT_BUTT};
			for(int i=0;i<4;i++)
			{
				m_arrayButton.push_back(new Button(buttonsLabel[i],Vector2f(window.getSize().x/2-60, window.getSize().y*2/7+80*i),buttonIndex[i]));
			}
		}
		break;
		case OPTION_MENU:
		{
			string buttonsLabel[]={"save","back","quit"};
			int buttonIndex[]={SAVE_BUTT,BACK_BUTT,QUIT_BUTT};
			for(int i=0;i<3;i++)
			{
				m_arrayButton.push_back(new Button(buttonsLabel[i],Vector2f(20+window.getSize().x*i/3,window.getSize().y*7/8),buttonIndex[i]));
			}
		}
		break;
		case GAME_CONTEXT:
		{
			Sprite * mapSprite = new Sprite();
			Texture t;// = new Texture();
			mapSprite->setTexture(t);
			mapSprite->setTextureRect(sf::IntRect(0, 0, m_mapWidth,m_mapHeight ));
			mapSprite->setPosition(Vector2f(m_mapPosX,m_mapPosY));
			m_arraySprite.push_back(mapSprite);

			int c=0;
			Element ** elmts = Element::elements();
			for(int i = 0 ; i< NB_MAX_ELEMENT; i++)
				if(elmts[i] != NULL && elmts[i]->isBuidable())
					m_arrayButton.push_back(new Button(this,((Buildable*)elmts[i])->getButtonTexture(),60,60,Vector2f(m_mapPosX + c++*70,m_mapPosY + m_mapHeight + 10),&GUI::buildElement,WAREHOUSE_TYPE));
			
			

			

		}
		break;
	}
}

void GUI::drawMap()
{
	m_mapdrawVal = (m_mapdrawVal+1)%2; //marker to know what has been drawn

	//TODO: create static access from ELEMENT

	for(int i = 0; i < m_mapHeight ; i++ )
	{
		for(int j = 0; j < m_mapWidth ; j++ )
		{
			if(m_mapDraw[i*m_mapWidth + j] != m_mapdrawVal)// if the pixel hasn't been drawn
			{
				m_mapDraw[i*m_mapWidth + j] = m_mapdrawVal; // mark the pixel as drawn
				if(m_map[i*m_mapWidth + j]!=0)//if a element is present on the pixel
				{
					Element * e = (Element*) m_map[i*m_mapWidth + j];//::elementsType[type-1];

					//draw element
					Sprite s = e->sprite();
					window.draw(s);

					int u_height = e->height();
					int u_width  = e->width();

					//draw HP
					/*Sprite life;
					Texture t;
					life.setTexture(t);
					life.setPosition(Vector2f(m_mapPosX+j+2,m_mapPosY+i+2));
					life.setTextureRect(sf::IntRect(0, 0, ((float)999/1000)*u_width-4, 8));
					life.setColor(sf::Color(255, 0, 0));
					window.draw(life);*/



					for(int iu = 0 ; iu < u_height ; iu++)//mark the whole area of the element as marked
						for(int ju = 0; ju < u_width ; ju++)
							m_mapDraw[(i+iu)*m_mapWidth + j + ju] = m_mapdrawVal;
				}
			}
		}
	}
}


void GUI::drawSelection()
{
	if(m_elementSelected!=NULL)
	{

		Sprite s = m_elementSelected->sprite();

		int height = m_elementSelected->height();
		int width  = m_elementSelected->width();

		//s.setTextureRect(sf::IntRect(0, 0, width,height ));
		s.setPosition(Vector2f(msPos.x-width/2,msPos.y-height/2));

		//if we're in the map
		if(msPos.x-width/2 > m_mapPosX && msPos.y-height/2 > m_mapPosY && msPos.x+width/2 < m_mapPosX+m_mapWidth && msPos.y+height/2 < m_mapPosY+m_mapHeight)
		{
			int x = msPos.x-width/2 - m_mapPosX;
			int y = msPos.y-height/2 - m_mapPosY;
			bool placeFree = true;

			//test the whole area of the element to know if the element is at a free place
			for(int iu = 0 ; iu < height ; iu++)
				for(int ju = 0; ju < width ; ju++)
					if(m_map[(y+iu)*m_mapWidth + x + ju] != 0)
						placeFree = false;

			if(placeFree)//if we're at free place
			{
				s.setColor(sf::Color(255, 255 , 255, 255));//m_elementSelected->color());
				m_elementOk = true;
			}
			else//if there're already somthing at the place
			{
				s.setColor(sf::Color(255, 0 , 0, 100));
				m_elementOk = false;
			}
		}
		//if we're outside the map     
		else
		{
			s.setColor(sf::Color(100, 0 , 0, 100));
			m_elementOk = false;
		}

		window.draw(s);
	}
}

