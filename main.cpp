#include <iostream>
#include <string>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "src/gamelib.hpp"

#include <time.h>

/*!
Run to start the graphic interface and play a game online or offline
*/
char* ip= "196.168.0.1";

int main(int argc,char ** argv)
{
	srand(time(NULL));
	if(argc>1)
		ip = argv[1];
	//The objetct game is able to run game offline/online graphic/nographic 
	Game game; 
	game.verbose();

	//initialize and start a graphic interface thread
	game.startGUI();

	//main loop updating the game state while it's active
	while(game.GUIactive()&&game.active())
		game.update(); //update the game

	return 0;
}
