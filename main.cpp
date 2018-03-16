#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "src/netlib.hpp"
#include "src/GUI/guilib.hpp"
#include "src/gamelib.hpp"

#include "vector"

#include <time.h> 


int main(int argc,char ** argv)
{
       srand(time(NULL));
       
       
       GUI gui;
       gui.getState()=GAME_CONTEXT;
       
       gui.start();
       
              
       Game game;
       //game.init();
       
       //game.menu();
       game.start();
             
       return 0;
}
