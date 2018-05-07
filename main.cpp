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
       
       cout << sizeof(int) << endl << sizeof(Element *) << endl << sizeof(long int ) << endl;
       
       Element * e = new Warehouse();
       unsigned long int eptr = (unsigned long int)e;
       cout << ((unsigned long int)NULL) << endl;
       
       Game game;
       
       game.verbose();
       
       //game.setOnline(2001);
       
       //game.connectToServer(2000,(char *)"127.0.0.1");
       //game.sentToServer((char*)"MR10V21W20");
       //game.sentToServer((char*)"Mhoyy");
       //game.sentToServer((char*)"Mhoazda");
       
       game.startGUI();
       
       
       
       while(game.GUIactive())
       {
              //get server info
              
              //update game
              game.update();
       }
       
       cout << "Game ended" <<endl;
             
       return 0;
}
