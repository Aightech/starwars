#include <iostream>
#include <string>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "src/netlib.hpp"
#include "src/GUI/guilib.hpp"
#include "src/gamelib.hpp"



#include <time.h> 


int main(int argc,char ** argv)
{
       srand(time(NULL));
       
       Game game;
       
       game.start();
       
       game.end();
             
       return 0;
}
