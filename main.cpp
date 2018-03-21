#include <iostream>
#include <string>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

//#include "src/netlib.hpp"
//#include "src/GUI/guilib.hpp"
#include "src/gamelib.hpp"

//#include "src/elements/element.hpp"



#include <time.h> 


int main(int argc,char ** argv)
{
       srand(time(NULL));
       
       //Element *el = new Warehouse();
       
       Game game;
       
       game.start();
       
       //game.updateElements();
       
       game.end();
             
       return 0;
}
