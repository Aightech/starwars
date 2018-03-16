#ifndef GUILIB_H
#define GUILIB_H

#include <iostream>
#include <string>
#include <vector>

#include "buttonlib.hpp"
#include "textboxlib.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define MAIN_MENU 0
#define OPTION_MENU 1
#define LAN_MENU 2
#define CREATION_MENU 3
#define GAME_ONLINE 4
#define GAME_CONTEXT 5

#define INQUIRY_GAME 6
#define DENOUNCE_GAME 7
#define GENPOLL_GAME 8
#define INIT_GAME 9
#define INQUIRY_GAME2 10

#define GAME_NAME "GAME"
#define WIN_W 1200
#define WIN_H 700

#define GAME_FONT_TITLE "media/fonts/SHERLOCKED.ttf"


#define GAME_FONT_BUTTON "media/fonts/SHERLOCKED.ttf"
#define GAME_THEME_BUTTON "media/theme/button.png"
#define GAME_THEME_ARRAY "media/theme/arrayConnection.png"

#define LANPORT 2027

#define PLAY_BUTT 1
#define LAN_BUTT 2
#define OPTION_BUTT 3
#define QUIT_BUTT 4
#define SAVE_BUTT 5
#define BACK_BUTT 6
#define HOST_BUTT 7
#define JOIN_BUTT 8
#define START_BUTT 9
#define REFRESH_BUTT 10

#define KILL_P1_BUTT 21
#define KILL_P2_BUTT 22
#define KILL_P3_BUTT 23
#define KILL_P4_BUTT 24

#define JOIN_S1_BUTT 31
#define JOIN_S2_BUTT 32
#define JOIN_S3_BUTT 33
#define JOIN_S4_BUTT 34

#define NB_MAX_PLAYERS 4




using namespace sf;
using namespace std;


class GUI
{
       /*! \class GUI
       * \brief This class represent the gui application.
       *      It owns the different methods running the gui.
       */
       public:
       GUI();
       
       int start();
       /*! \brief Accessor method to get the state of the game from the main loop.   */
       int& getState(){return m_state;};
       
       
       private:
       /*! \brief Generate the graphics of the different menu's states of the game. It's mainly SFML sprites/Text/Buttons creation   */
       void createContext();
       
       void drawMap();
       
       
       RenderWindow window;
       
       vector<Sprite*> m_arraySprite;
       vector<Text*>   m_arrayText;
       vector<Font*>   m_arrayFont;
       
       vector<Button*> m_arrayButton;
       vector<Textbox*> m_arrayTextBox;
       
       Vector2i msPos;
       
       int m_state;
       int m_elementSelected;
       
       unsigned int *m_map;
       int m_mapHeight;
       int m_mapWidth;
       
       int m_mapPosX;
       int m_mapPosY;
       
       unsigned char *m_mapDraw;
       int m_mapdrawVal;
       
       
};



#endif
