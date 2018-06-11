#include <iostream>
#include <string>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "src/gamelib.hpp"

#include <time.h>

/*!
Run to start the graphic interface and play a game online or offline
*/


int main(int argc,char ** argv)
{
	srand(time(NULL));

	Game game; 

	//game.verbose();
	

	//game.setOnline(atoi(argv[1])); //comment if offline
	

	//game.connectToServer(2000,(char *)"127.0.0.1"); //comment if offline

	game.startGUI(); //start the graphic interface

	while(game.GUIactive())
	{
		game.update(); //update the game
	}

	cout << "Game ended" <<endl;
	
	return 0;
}
