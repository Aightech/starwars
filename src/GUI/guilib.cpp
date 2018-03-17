#include "guilib.hpp"

#include <iostream>
#include <string>
#include <vector>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

GUI::GUI()
{
       
       Button::setFont(GAME_FONT_BUTTON);
       Button::setTexture(GAME_THEME_BUTTON);
       
       window.create(VideoMode(WIN_W,WIN_H), GAME_NAME); 
       
       m_state=0;
       m_elementSelected=0;
       
       m_mapHeight = 600;
       m_mapWidth = 950;
       
       m_map = new unsigned int[m_mapHeight*m_mapWidth] ();
       m_mapDraw = new unsigned char[m_mapHeight*m_mapWidth] ();
       m_mapdrawVal = 0;
       
       m_mapPosX = window.getSize().x-m_mapWidth -10;
       m_mapPosY = 10;
       
       
       
         

}

int GUI::start()
{
       createContext();
       
       
       while(window.isOpen() && m_state!=GAME_ONLINE)
       {
              
              
              Event event;
              while(window.pollEvent(event))
              {
                     if(event.type==Event::Closed)
                            window.close();
                     if (event.type == sf::Event::Resized)
                     {
                            window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                            createContext();
                     }
                     if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                     {
                         
                            if(m_elementSelected!=0)
                            {
                                   int e_height[3]= {50,50,30};
                                   int e_width[3] = {50,50,30};
                                   int height = e_height[m_elementSelected-1];
                                   int width  = e_width[m_elementSelected-1];


                                   m_map[(msPos.y-height/2-m_mapPosY)*m_mapWidth + msPos.x-width/2-m_mapPosX]=m_elementSelected;
                                   m_elementSelected=0;
                            }
                            for(int i=0;i<3;i++)
                                   if(msPos.x > m_mapPosX +80*i && msPos.x < m_mapPosX + 60 +80*i && msPos.y > m_mapPosY + m_mapHeight && msPos.y < m_mapPosY + m_mapHeight + 60)
                                          m_elementSelected = i+1;
                                          
                            
                         
                     }
                     if (event.type == sf::Event::TextEntered)
                     {
                         //if (event.text.unicode < 128)
                         char c=event.text.unicode;
                             std::cout << "ASCII character typed: " << (int)c << std::endl;
                     }
                     
              }
              
              msPos = Mouse::getPosition(window);
              
              
              window.clear(Color(48,48,48));
              
              
              for(int i=0;i<m_arraySprite.size();i++)
                     window.draw(*m_arraySprite[i]);
              for(int i=0;i<m_arrayText.size();i++)
                     window.draw(*m_arrayText[i]);
              
              //std::cout << m_elementSelected-1 << endl;
              drawUnitSelection();
              drawMap();
              
             //int nbButt=button.getNbOfBt();
             int select=0;
             int s=0;
             
              for(int i=0;i<m_arrayButton.size();i++)
                     select+=m_arrayButton[i]->update(window);
              if(select)
              {
                     switch(abs(select))
                     {
                            case PLAY_BUTT:
                            
                            break;
                            case LAN_BUTT:
                                   //m_state=LAN_MENU;
                                   
                                   //m_nbOfServers=scanServers(m_servers);
                                   
                                   //if(m_nbOfServers==0)
                                     //   printf("no server\n");  
                                   //createMenu();
                            break;
                            
                            
                            case JOIN_S1_BUTT:
                            case JOIN_S2_BUTT:
                            case JOIN_S3_BUTT:
                            case JOIN_S4_BUTT:
                            {
                            
                                   int server=abs(select)-JOIN_S1_BUTT;
                                   //printf("G: server :%d\n",server);
                                   
                                   //sprintf(m_buffer.Tx,"G%dP%dN%s",1,m_buffer.R_port,"aight;");
                                   //m_buffer.T_flag=1;
                                   //sendTCP(m_servers[m_server].IPaddress,m_servers[m_server].portNo,&m_buffer); 
                                   //m_host=0; 
                                     //m_buffer.R_flag=0;
                                   //if(atoi(strchr(m_buffer.Rx,'G')+1)==1)
                                   m_state=CREATION_MENU;
                            }             
                            break;
                            
                            case HOST_BUTT:
                            {
                                   m_state=CREATION_MENU;
                                   /*m_host=1;
                                   m_nbPlayer=1;
                                   strcpy(m_players[0].name,"mwa");
                                   strcpy(m_players[0].IPaddress,"127.0.0.1");
                                   m_players[0].portNo=m_buffer.R_port;
                                   m_players[0].no=0;
                                   
                                   if(!fork())
                                   {
                                          startServer();
                                          exit(0);
                                   }
                                   
                                   m_server=0;
                                   strcpy(m_servers[0].IPaddress,"127.0.0.1");
                                   m_servers[0].portNo=SERVERPORT;
                                   */
                                   
                                   createContext();
                            }
                            break;
                            
                            case KILL_P1_BUTT: 
                            case KILL_P2_BUTT: 
                            case KILL_P3_BUTT: 
                            case KILL_P4_BUTT:
                            { 
                                   /*int toKill = abs(select)-KILL_P1_BUTT;
                                   printf("G: s : %d\n",m_server);
                                   printf("G: Killing : %d , to [%s] on %d\n",toKill,m_servers[m_server].IPaddress,m_servers[m_server].portNo);
                                   sprintf(m_buffer.Tx,"G%dU%d",10,toKill);
                                   m_buffer.T_flag=1;
                                   sendTCP(m_servers[m_server].IPaddress,m_servers[m_server].portNo,&m_buffer); 
                                   */
                            }            
                            break;
                            
                            case START_BUTT:
                            { 
                                   /*if(m_nbPlayer>1)
                                   {
                                          sprintf(m_buffer.Tx,"G%d",20);
                                          m_buffer.T_flag=1;
                                          sendTCP(m_servers[m_server].IPaddress,m_servers[m_server].portNo,&m_buffer);
                                          
                                          
                                   }*/
                                   
                                   
                                   
                            }            
                            break;
                            
                            
                            case OPTION_BUTT:
                                   m_state=OPTION_MENU;
                                   createContext();
                            break;
                            case QUIT_BUTT:
                                   window.close();
                            break;
                            
                            case BACK_BUTT:
                            {
                                   
                                   /*if(m_state=CREATION_MENU)
                                   {
                                          sprintf(m_buffer.Tx,"G%dU%d",10,m_myNo);
                                          m_buffer.T_flag=1;
                                          sendTCP(m_servers[m_server].IPaddress,m_servers[m_server].portNo,&m_buffer); 
                                   }*/
                                   m_state=MAIN_MENU;
                                   
                                   createContext();
                            }
                            break;
                            
                            case SAVE_BUTT:
                            break;
                     }
              }
              
              //window.draw(m_title);
              window.display();
       }
       
       return 0;
       
}

void GUI::createContext()
{
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
                     
                     
                     Sprite * warehouseSrpite = new Sprite();
                     warehouseSrpite->setTexture(t);
                     int warehouseHeight= 60;
                     int warehouseWidth= 60;
                     warehouseSrpite->setTextureRect(sf::IntRect(0, 0, warehouseWidth, warehouseHeight ));
                     warehouseSrpite->setPosition(Vector2f(m_mapPosX,m_mapPosY + m_mapHeight + 10));
                     warehouseSrpite->setColor(sf::Color(0, 150, 0));
                     m_arraySprite.push_back(warehouseSrpite);
                     
                     Sprite * farmSrpite = new Sprite();
                     farmSrpite->setTexture(t);
                     int farmHeight= 60;
                     int farmWidth= 60;
                     farmSrpite->setTextureRect(sf::IntRect(0, 0, farmWidth, farmHeight ));
                     farmSrpite->setPosition(Vector2f(m_mapPosX + 80,m_mapPosY + m_mapHeight + 10));
                     farmSrpite->setColor(sf::Color(0, 200, 0));
                     m_arraySprite.push_back(farmSrpite);
                     
                     Sprite * towerSrpite = new Sprite();
                     towerSrpite->setTexture(t);
                     int towerHeight= 60;
                     int towerWidth= 60;
                     towerSrpite->setTextureRect(sf::IntRect(0, 0, towerWidth, towerHeight ));
                     towerSrpite->setPosition(Vector2f(m_mapPosX + 160,m_mapPosY + m_mapHeight + 10));
                     towerSrpite->setColor(sf::Color(0, 250, 0));
                     m_arraySprite.push_back(towerSrpite);
                     
              }
              break;
              /*case LAN_MENU:
              {
                     string buttonsLabel[]={"host game","refresh","back"};
                     int buttonIndex[]={HOST_BUTT,REFRESH_BUTT,BACK_BUTT};
                     for(int i=0;i<3;i++)
                     {
                            buttons.push_back(new Button(buttonsLabel[i],Vector2f(20+window.getSize().x*i/3,window.getSize().y*7/8),buttonIndex[i]));
                     }
                     
                     m_arraySprites.push_back(new Sprite);
                     m_arraySprites[0]->setTexture(m_arrayConnectionTexture);
                     m_arraySprites[0]->setTextureRect(sf::IntRect(0, 0, 651,37 ));
                     m_arraySprites[0]->setPosition(Vector2f(window.getSize().x/2-350,window.getSize().y*3/10));
                     
                     for(int i=0;i<m_nbOfServers;i++)
                     {      
                            int n=5;
                            
                            m_arraySprites.push_back(new Sprite);
                            m_arraySprites[i+1]->setTexture(m_arrayConnectionTexture);
                            m_arraySprites[i+1]->setTextureRect(sf::IntRect(1, 38, 649,50 ));
                            m_arraySprites[i+1]->setPosition(Vector2f(window.getSize().x/2-350+1,window.getSize().y*3/10+37+50*i));
                            
                            //Server number
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i]->setFont(m_font);
                            char no[]="1";
                            no[0]='1'+i;
                            m_arrayText[n*i]->setString(no);
                            m_arrayText[n*i]->setCharacterSize(15);
                            m_arrayText[n*i]->setPosition(Vector2f(window.getSize().x/2-330,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i]->setColor(Color(0,0,0));
                            
                            //Server name
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+1]->setFont(m_font);
                            m_arrayText[n*i+1]->setString(m_servers[i].name);
                            m_arrayText[n*i+1]->setCharacterSize(15);
                            m_arrayText[n*i+1]->setPosition(Vector2f(window.getSize().x/2-300,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+1]->setColor(Color(0,0,0));
                            
                            //Server IPaddress
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+2]->setFont(m_font);
                            m_arrayText[n*i+2]->setString(m_servers[i].IPaddress);
                            m_arrayText[n*i+2]->setCharacterSize(15);
                            m_arrayText[n*i+2]->setPosition(Vector2f(window.getSize().x/2-100,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+2]->setColor(Color(0,0,0));
                            
                            //Server Port no
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+3]->setFont(m_font);
                            char port[]="20270000";
                            sprintf(port,"%d",m_servers[i].portNo);
                            m_arrayText[n*i+3]->setString(port);
                            m_arrayText[n*i+3]->setCharacterSize(15);
                            m_arrayText[n*i+3]->setPosition(Vector2f(window.getSize().x/2+70,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+3]->setColor(Color(0,0,0));
                            
                            //Server date
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+4]->setFont(m_font);
                            char date[]="17h25m23";
                            m_arrayText[n*i+4]->setString(date);
                            m_arrayText[n*i+4]->setCharacterSize(15);
                            m_arrayText[n*i+4]->setPosition(Vector2f(window.getSize().x/2+170,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+4]->setColor(Color(0,0,0));
                            
                            buttons.push_back(new Button("join",Vector2f(window.getSize().x/2+305,window.getSize().y*3/10+35+50*i),JOIN_S1_BUTT+i));
                     }
                     
              }
              break;
              case CREATION_MENU:
              {
                     string buttonsLabel[]={"start","back"};
                     int buttonIndex[]={START_BUTT,BACK_BUTT};
                     for(int i=0;i<2;i++)
                     {
                            buttons.push_back(new Button(buttonsLabel[i],Vector2f(20+window.getSize().x*7/9,window.getSize().y*(i+8)/10),buttonIndex[i]));
                     }
                     if(!m_host)
                            buttons[0]->disable();
                     m_arraySprites.push_back(new Sprite);
                     m_arraySprites[0]->setTexture(m_arrayConnectionTexture);
                     m_arraySprites[0]->setTextureRect(sf::IntRect(0, 0, 651,37 ));
                     m_arraySprites[0]->setPosition(Vector2f(window.getSize().x/2-350,window.getSize().y*3/10));
                     
                     for(int i=0;i<m_nbPlayer;i++)
                     {      
                            int n=5;
                            
                            m_arraySprites.push_back(new Sprite);
                            m_arraySprites[i+1]->setTexture(m_arrayConnectionTexture);
                            m_arraySprites[i+1]->setTextureRect(sf::IntRect(1, 38, 649,50 ));
                            m_arraySprites[i+1]->setPosition(Vector2f(window.getSize().x/2-350+1,window.getSize().y*3/10+37+50*i));
                            
                            //Player number
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i]->setFont(m_font);
                            char no[]="1";
                            no[0]='1'+i;
                            m_arrayText[n*i]->setString(no);
                            m_arrayText[n*i]->setString(no);
                            m_arrayText[n*i]->setCharacterSize(15);
                            m_arrayText[n*i]->setPosition(Vector2f(window.getSize().x/2-330,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i]->setColor(Color(0,0,0));
                            
                            //Player name
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+1]->setFont(m_font);
                            m_arrayText[n*i+1]->setString(m_players[i].name);
                            m_arrayText[n*i+1]->setCharacterSize(15);
                            m_arrayText[n*i+1]->setPosition(Vector2f(window.getSize().x/2-300,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+1]->setColor(Color(0,0,0));
                            
                            //Player IPaddress
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+2]->setFont(m_font);
                            char IP[]="192.168.0.1";
                            m_arrayText[n*i+2]->setString(m_players[i].IPaddress);
                            m_arrayText[n*i+2]->setCharacterSize(15);
                            m_arrayText[n*i+2]->setPosition(Vector2f(window.getSize().x/2-100,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+2]->setColor(Color(0,0,0));
                            
                            //Player Port no
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+3]->setFont(m_font);
                            char port[]="20270000";
                            sprintf(port,"%d",m_players[i].portNo);
                            m_arrayText[n*i+3]->setString(port);
                            m_arrayText[n*i+3]->setCharacterSize(15);
                            m_arrayText[n*i+3]->setPosition(Vector2f(window.getSize().x/2+70,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+3]->setColor(Color(0,0,0));
                            
                            //Player Port no
                            m_arrayText.push_back(new Text);
                            m_arrayText[n*i+4]->setFont(m_font);
                            char date[]="17h25m23";
                            m_arrayText[n*i+4]->setString(date);
                            m_arrayText[n*i+4]->setCharacterSize(15);
                            m_arrayText[n*i+4]->setPosition(Vector2f(window.getSize().x/2+170,window.getSize().y*3/10+50+50*i));
                            m_arrayText[n*i+4]->setColor(Color(0,0,0));
                            
                            buttons.push_back(new Button("X",Vector2f(window.getSize().x/2+305,window.getSize().y*3/10+35+50*i),KILL_P1_BUTT+i));
                            if(!m_host)
                                   buttons[buttons.size()-1]->disable();
                     }
                     
              }
              break;*/
       }
}

void GUI::drawMap()
{
       m_mapdrawVal = (m_mapdrawVal+1)%2; //marker to know what has been drawn
       int e_height[3]= {50,50,30};
       int e_width[3] = {50,50,30};
       for(int i = 0; i < m_mapHeight ; i++ )
       {
              for(int j = 0; j < m_mapWidth ; j++ )
              {
                     if(m_mapDraw[i*m_mapWidth + j] != m_mapdrawVal)// if the pixel hasn't been drawn
                     {
                            m_mapDraw[i*m_mapWidth + j] = m_mapdrawVal; // mark the pixel as drawn
                            if(m_map[i*m_mapWidth + j]!=0)//if a elemen is present on the pixel
                            {
                                   int type = m_map[i*m_mapWidth + j];
                                   //cout << type << endl;
                                   int u_height = e_height[type-1];
                                   int u_width  = e_width[type-1];
                                   Sprite s;
                                   Texture t;
                                   s.setTexture(t);
                                   
                                   s.setTextureRect(sf::IntRect(0, 0, u_width, u_height));
                                   s.setPosition(Vector2f(m_mapPosX+j,m_mapPosY+i));
                                   s.setColor(sf::Color(0, 100 + 50*type , 0));
                                   window.draw(s);
                                   
                                   s.setPosition(Vector2f(m_mapPosX+j+2,m_mapPosY+i+2));
                                   s.setTextureRect(sf::IntRect(0, 0, u_width-4, 8));
                                   s.setColor(sf::Color(255, 0, 0));
                                   window.draw(s);
                                   
                                   for(int iu = 0 ; iu < u_height ; iu++)//mark the whole area of the element as marked
                                          for(int ju = 0; ju < u_width ; ju++)
                                                 m_mapDraw[(i+iu)*m_mapWidth + j + ju] = m_mapdrawVal;
                            }
                     }
              }
       }
}


void GUI::drawUnitSelection()
{
       //m_mapPosX + 160,m_mapPosY + m_mapHeight + 10
       
       int e_height[3]= {50,50,30};
       int e_width[3] = {50,50,30};
       if(m_elementSelected)
       {
              
              Sprite s;
              Texture t;// = new Texture();
              s.setTexture(t);
              
              int height = e_height[m_elementSelected-1];
              int width = e_width[m_elementSelected-1];
              s.setTextureRect(sf::IntRect(0, 0, width,height ));
              s.setPosition(Vector2f(msPos.x-width/2,msPos.y-height/2));
              s.setColor(sf::Color(0, 100 + 50*m_elementSelected, 0));
              
              window.draw(s);
       }
}

