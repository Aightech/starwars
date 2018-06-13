#include "guilib.hpp"
#include "../gamelib.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>

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
	sf::Image icon;
	icon.loadFromFile("media/theme/Millennium-falcon-icon.png");
	window.setIcon(32,32,icon.getPixelsPtr());

	//////------- MAP SETTING ------- /////
	m_map = map;//get and share the game map
	m_mapHeight = height;
	m_mapWidth = width;
	m_mapPosX = window.getSize().x/2-m_mapWidth/2;//set the position of the map on the window
	m_mapPosY = 20;
	/////--------ELEMENT SETING --------/////
	Element::mapOffsetY()=m_mapPosY;
	Element::mapOffsetX()=m_mapPosX;



	//////------- FLAGS SETTING ------- /////
	m_state=0;
	m_elementSelectedType=0;
	m_mapDraw = new unsigned char[m_mapHeight*m_mapWidth] ();//used in drawMap
	m_mapdrawVal = 0;

	m_hasBeenAnimated = false;

}

//this methode is launch in a thread
int GUI::start(void *pgame)
{       
	//get the game object
	m_game = (Game *) pgame;

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
			
			/////------- KEYBOARD EVENT ------- /////
			if (event.type == sf::Event::TextEntered)
			{
				//if (event.text.unicode < 128)
				char c=event.text.unicode;
				std::cout << "ASCII character typed: " << (int)c << std::endl;
				if(c=='p')
				{
					std::cout << "Attack " << std::endl;
					//m_game->test();
				}
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if(m_elementSelectedType!=0 && m_elementOk)
				{
					cout << "CLICKED" << endl;
					Request r={m_elementSelectedType,msPos.x-m_mapPosX-m_elementSelected->width()/2,msPos.y-m_mapPosY-m_elementSelected->height()/2};
					r.val3 = -1;
					r.p=m_playerTurn;
					m_game->request(&r);
					delete m_elementSelected;
					m_elementSelected = NULL;

					m_elementOk=false;
					m_elementSelectedType=0;
					
					for(int i=0;i<m_arrayButton.size();i++)
						m_arrayButton[i]->clicked()=false;
				
				}
			}

		}
		/////------- MOUSE EVENT ------- /////
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if(	m_state == GAME_CONTEXT && msPos.x > m_mapPosX && msPos.y > m_mapPosY &&
				msPos.x< m_mapPosX+m_mapWidth && msPos.y < m_mapPosY+m_mapHeight	)
			{
				
				m_mouseClicked++;
			}

		}
		else
		{
			if(m_mouseClicked > 0 && m_mouseClicked < 50)
			{
				Request r={R_TARGET,msPos.x-m_mapPosX,msPos.y-m_mapPosY};
				r.val3 = -1;
				cout << "set:"<< m_arraySelectedElements.size() << std::endl;
				
				for(int i=0 ; i<m_arraySelectedElements.size();i++)
				{
					if(m_arraySelectedElements[i] != NULL)
					{
						cout << "set:"<<m_arraySelectedElements[i] << std::endl;
						r.e= (unsigned long int)m_arraySelectedElements[i]->no();
						m_game->request(&r);
					}
					//m_arraySelectedElements[i]->setTarget(msPos.x-m_mapPosX,msPos.y-m_mapPosY);
				}
				//m_arraySelectedElements[i]->setTarget(msPos.x-m_mapPosX,msPos.y-m_mapPosY);
			}
			m_mouseClicked=0;
			
		}

		/////-------  GUI UPDATE ------- /////

		//get the mouse position
		msPos = Mouse::getPosition(window);

		
		int select=update();
		
	}

	m_game->endGUI();
	return 0;
}

void GUI::buildElement(int type)
{
	if(m_elementSelected!=NULL)
		delete m_elementSelected;
	m_elementSelected = Element::factory(type, -1, msPos.x-50/2, msPos.y-50/2,(Player *)m_playerTurn);//new Warehouse(-1,msPos.x-50/2,msPos.y-50/2);
	m_elementSelectedType = type;
}

void GUI::menu(int select)
{
	
	switch(abs(select))
	{
		case PLAY_BUTT:
			m_game->addPlayer(0);
			m_game->addPlayer(1);
			m_state=GAME_CONTEXT;
			m_hasBeenAnimated=false;
			createContext();
		break;
		case LAN_BUTT:
			m_state=LAN_MENU;
			createContext();
		break;


		case JOIN_BUTT:
		{
			m_game->setOnline(2001); //comment if offline
			m_playerTurn = m_game->connectToServer(2000,(char *)"127.0.0.1"); //comment if offline
			if(m_playerTurn == 0 || m_playerTurn == 1)
			{
				m_game->addPlayer(m_playerTurn);
				m_state=GAME_CONTEXT;
			}
			else 
			{
				m_playerTurn = 0;
				m_state=LAN_MENU;
			}
			m_hasBeenAnimated=false;
			createContext();
		}
		break;

		case HOST_BUTT:
		{
			system("./server &");
			m_game->setOnline(2001); //comment if offline
			m_playerTurn = m_game->connectToServer(2000,(char *)"127.0.0.1"); //comment if offline
			if(m_playerTurn == 0 || m_playerTurn == 1)
			{
				m_game->addPlayer(m_playerTurn);
				m_state=GAME_CONTEXT;
			}
			else 
			{
				m_playerTurn = 0;
				m_state=LAN_MENU;
			}
			m_hasBeenAnimated=false;
			createContext();
		}
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
	update();
	window.display();
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
	for(int i=m_arrayAnimation.size()-1;i>-1;i--)
	{
		delete m_arrayAnimation[i];
		m_arrayAnimation.pop_back();
	}
	for(int i=m_arrayText.size()-1;i>-1;i--)
	{
		delete m_arrayText[i];
		m_arrayText.pop_back();
	}
	///
	///BACKGROUND
//	Sprite * background = new Sprite();
//	Texture tb;// = new Texture();
//	background->setTexture(tb);
//	if (!tb.loadFromFile("media/theme/background.jpg"))
//		{
//			cout << "Erreur chargement image!"<< endl;
//			// return
//		}
//	background->setTextureRect(sf::IntRect(0, 0, window.getSize().x,window.getSize().y));
	m_arrayAnimation.push_back(new Animation("media/theme/background.jpg",0,0,window.getSize().x,window.getSize().y));

	///

	/////------- CREATION F CONTEXTS ------- /////
	switch(m_state)
	{
		case MAIN_MENU:
		{
			
			m_arrayAnimation.push_back(new Animation("media/theme/game_logo.png",110,30));
			m_arrayAnimation.push_back(new Animation("media/theme/porg_232.png",200,400,232,233,4));
			
			string buttonsLabel[]={"play","lan","options","quit"};
			int buttonIndex[]={PLAY_BUTT,LAN_BUTT,OPTION_BUTT,QUIT_BUTT};
			for(int i=0;i<4;i++)
			{
				m_arrayButton.push_back(new Button(this,buttonsLabel[i],Vector2f(window.getSize().x*3/4-60, window.getSize().y*4/7+80*i-300),buttonIndex[i],&GUI::menu));
			}
			if(!m_hasBeenAnimated)
			{
				int nbInc = 20;
				for(int j = 0; j< nbInc+1; j++)
				{
					for(int i=0;i<4;i++)
						m_arrayButton[i]->setPosition(Vector2f(window.getSize().x*3/4-60, window.getSize().y*4/7*j/nbInc+80*i-300));
					update();
					waitSec(1.5/nbInc);
				}
				m_hasBeenAnimated=true;
			}			

		}
		break;
		case LAN_MENU:
		{
			//std::cout << "Menu " << std::endl<< std::endl<< std::endl;
			m_arrayAnimation.push_back(new Animation("media/theme/porg_232.png",200,400,232,233,4));
			string buttonsLabel[]={"host","join","back"};
			int buttonIndex[]={HOST_BUTT,JOIN_BUTT,BACK_BUTT};
			for(int i=0;i<3;i++)
			{
				m_arrayButton.push_back(new Button(this,buttonsLabel[i],Vector2f(window.getSize().x*(1+i)/5,window.getSize().y*7/8),buttonIndex[i],&GUI::menu));
			}
		}
		break;
		case OPTION_MENU:
		{
			//std::cout << "Menu " << std::endl<< std::endl<< std::endl;
			m_arrayAnimation.push_back(new Animation("media/theme/porg_232.png",200,400,232,216,4));
			string buttonsLabel[]={"save","back","quit"};
			int buttonIndex[]={SAVE_BUTT,BACK_BUTT,QUIT_BUTT};
			for(int i=0;i<3;i++)
			{
				m_arrayButton.push_back(new Button(this,buttonsLabel[i],Vector2f(window.getSize().x*(1+i)/5,window.getSize().y*7/8),buttonIndex[i],&GUI::menu));
			}
		}
		break;
		case GAME_CONTEXT:
		{
			
			m_mapPosX = window.getSize().x/2-m_mapWidth/2;
			m_arrayAnimation.push_back(new Animation("media/theme/map.png",m_mapPosX,m_mapPosY,m_mapWidth,m_mapHeight));
			cout << "w : " << m_mapPosX << endl << " h : " << m_mapPosY << endl;
			
			m_arrayAnimation.push_back(new Animation("media/theme/player1.png",m_mapPosX/2-130/2 ,m_mapPosY,130,130));
			m_arrayAnimation.push_back(new Animation("media/theme/ressource.png",m_mapPosX/2-130/2 ,m_mapPosY+135,130,60));
			Text* t=new Text();
			t->setString("12");
			t->setColor(Color(255,255,255));
			t->setPosition(Vector2f(m_mapPosX/2,m_mapPosY+135));
			m_arrayText.push_back(t);
			m_arrayAnimation.push_back(new Animation("media/theme/pop1.png",m_mapPosX/2-130/2 ,m_mapPosY+200,130,60));

			m_arrayAnimation.push_back(new Animation("media/theme/player2.png",m_mapPosX + m_mapWidth+ m_mapPosX/2-130/2,m_mapPosY,130,130));
			m_arrayAnimation.push_back(new Animation("media/theme/ressource.png",m_mapPosX + m_mapWidth+ m_mapPosX/2-130/2,m_mapPosY+135,130,60));
			m_arrayAnimation.push_back(new Animation("media/theme/pop2.png",m_mapPosX + m_mapWidth+ m_mapPosX/2-130/2 ,m_mapPosY+200,130,60));
			
			//m_arrayAnimation.push_back(new Animation("media/theme/doorSolo.png",window.getSize().x/2,0,window.getSize().x,window.getSize().y));
			
			int c=0;
			Element ** elmts = Element::elements();
			for(int i = 0 ; i< NB_MAX_ELEMENT; i++)
				if(elmts[i] != NULL && elmts[i]->isBuidable())
					m_arrayButton.push_back(new Button(this,((Buildable*)elmts[i])->getButtonTexture(),60,60,Vector2f(m_mapPosX + c++*70,m_mapPosY + m_mapHeight + 10),&GUI::buildElement,elmts[i]->type(),elmts[i]->getInfo()));
			
			if(!m_game->isOnline())
				m_arrayButton.push_back(new Button(this,"next",Vector2f(m_mapPosX/2-130/2, m_mapPosY + m_mapHeight + 17),0,&GUI::nextButton));
			
			m_nbBuildable = (!m_game->isOnline())?++c:c;
			
			c=0;
			for(int i = 0 ; i< NB_MAX_ELEMENT; i++)
				if(elmts[i] != NULL && elmts[i]->isBuidable())
					m_arrayButton.push_back(new Button(this,((Buildable*)elmts[i])->getButtonTexture(1),60,60,Vector2f(m_mapPosX + m_mapWidth -60 - c++*70,m_mapPosY + m_mapHeight + 10),&GUI::buildElement,elmts[i]->type(),elmts[i]->getInfo()));
			

			if(!m_game->isOnline())
				m_arrayButton.push_back(new Button(this,"next",Vector2f(m_mapPosX + m_mapWidth+ m_mapPosX/2-130/2,m_mapPosY + m_mapHeight + 17),1,&GUI::nextButton));
				
			for(int i =m_nbBuildable*(1-m_playerTurn); i< m_nbBuildable*(2-m_playerTurn) ; i++)
				m_arrayButton[i]->disable();
			
			if(!m_hasBeenAnimated && m_game->isOnline())
			{
				m_arrayAnimation.push_back(new Animation("media/theme/doorSolo.png",window.getSize().x,0,window.getSize().x,window.getSize().y));
				if(m_playerTurn==1)
					m_arrayAnimation.back()->setScale(-1.,1.);
				int nbInc = 20;
				for(int j = 0; j< nbInc+1; j++)
				{
					if(m_playerTurn==0)
						m_arrayAnimation.back()->setPosition(Vector2f(window.getSize().x*(2-((float)sqrt(j))/sqrt(nbInc))/2,0));
					if(m_playerTurn==1)
						m_arrayAnimation.back()->setPosition(Vector2f(window.getSize().x*(((float)sqrt(j))/sqrt(nbInc))/2,0));
					update();
					waitSec(1.5/nbInc);
				}
				m_hasBeenAnimated=true;
			}

			
		}
		break;
		
	}
}

int GUI::update()
{
	window.clear(Color(48,48,48));
	for(int i=0;i<m_arrayAnimation.size();i++)
		m_arrayAnimation[i]->update(window);

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
	
	m_arrayAnimation[m_arrayAnimation.size()-1]->update(window);
	
	window.display();
	return select;
}

void GUI::drawMap()
{
	m_mapdrawVal = (m_mapdrawVal+1)%2; //marker to know what has been drawn

	//TODO: create static access from ELEMENT
	if(m_mouseClicked > 50)
		for(int i=m_arraySelectedElements.size()-1 ; i>=0;i--)
			m_arraySelectedElements.pop_back();

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
					m_game->elmtMtx().lock();
					Sprite s = e->sprite();
					if(m_mouseClicked>50 && e->player()->no()==m_playerTurn)
					{
						if(m_selectionRect.getGlobalBounds().intersects(s.getGlobalBounds()))
						{
							e->select();//cout<<"hey"<<endl;
							m_arraySelectedElements.push_back(e);
							//m_arraySelectedElements[0]->no();
						}
						else
							e->unselect();
					}
					window.draw(s);
					m_game->elmtMtx().unlock();
					
					
					int u_height = e->height();
					int u_width  = e->width();

					//draw HP
					Sprite life;
					Texture t;
					life.setTexture(t);
					float hps = ((float)e->HP())/20;
					//cout << e->HP() << endl;
					life.setPosition(Vector2f(m_mapPosX+j+(u_width-hps)/2,m_mapPosY+i+2));
					life.setTextureRect(sf::IntRect(0, 0, hps, 4));
					life.setColor(sf::Color(0, 255, 0));
					window.draw(life);



					for(int iu = 0 ; iu < u_height ; iu++)//mark the whole area of the element as marked
						for(int ju = 0; ju < u_width ; ju++)
							m_mapDraw[(i+iu)*m_mapWidth + j + ju] = m_mapdrawVal;
				}
			}
		}
	}
	if(m_arraySelectedElements.size()>0)
		m_arraySelectedElements[0]->targetAnimation().update(window);
	
//	cout  << m_mouseClicked<< "-----------------"<<endl;
//	for(int i=0 ; i<m_arraySelectedElements.size();i++)
//		cout  << m_mouseClicked<< "get:"<<m_arraySelectedElements[i] << std::endl;
//	cout << m_mouseClicked << "-----------------"<<endl;
}


void GUI::drawSelection()
{
	if(m_mouseClicked==5)
	{
			Texture t;
			m_selectionRect.setTexture(t);
			m_selectionRect.setTextureRect(sf::IntRect(0, 0, 5,5 ));
			m_startSelection = Vector2f(msPos.x,msPos.y);
			m_selectionRect.setPosition(Vector2f(msPos.x,msPos.y));
			m_selectionRect.setColor(Color(255,0,0,100));
			window.draw(m_selectionRect);
	}
	else if(m_mouseClicked==50)
		for(int i=m_arraySelectedElements.size()-1 ; i>-1;i--)
		{
			m_arraySelectedElements[i]->unselect();
			m_arraySelectedElements.pop_back();
		}
	if(m_mouseClicked>50)
	{
		if(msPos.x> m_mapPosX && msPos.y > m_mapPosY && msPos.x < m_mapPosX+m_mapWidth && msPos.y < m_mapPosY+m_mapHeight)
		{
			float dx = msPos.x-m_startSelection.x;
			float dy = msPos.y-m_startSelection.y;
			
			m_selectionRect.setPosition(Vector2f( (dx>0)?m_startSelection.x:msPos.x, (dy>0)?m_startSelection.y:msPos.y));
			m_selectionRect.setTextureRect(sf::IntRect(0, 0, dx ,dy ));
		}
		
		window.draw(m_selectionRect);
	}
	
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

void GUI::nextButton(int playerNo)
{
	m_game->setTurn(playerNo);
	for(int i =m_nbBuildable*playerNo; i< m_nbBuildable*(1+playerNo) ; i++)
		m_arrayButton[i]->disable();
		
	for(int i =m_nbBuildable*(1-playerNo); i< m_nbBuildable*(2-playerNo) ; i++)
		m_arrayButton[i]->enable();
		
	for(int i=m_arraySelectedElements.size()-1 ; i>-1;i--)
	{
		m_arraySelectedElements[i]->unselect();
		m_arraySelectedElements.pop_back();
	}
	
	m_playerTurn=1-playerNo;
}

