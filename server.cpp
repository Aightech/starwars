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
       
       Game game;
       game.verbose();
       game.setConnectable();
       
       game.setOnline(2000);
       
       //game.startGUI();
       char enter[1024];
       int n;
       
       while(1)
       {
              
              //update game
              //game.update();
               while((n=game.getReceiverBuffer(enter))>-1)
              {
                     
              }
       }
       
       cout << "Game ended" <<endl;
             
       return 0;
}
