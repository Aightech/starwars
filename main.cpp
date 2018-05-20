#include <iostream>
#include <string>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "src/gamelib.hpp"

#include <time.h>



int main(int argc,char ** argv)
{
	srand(time(NULL));

	Game game;

	game.verbose();
	

	game.setOnline(atoi(argv[1]));
	

	game.connectToServer(2000,(char *)"127.0.0.1");

	game.startGUI();

	while(game.GUIactive())
	{//update game
		game.update();
	}

	cout << "Game ended" <<endl;
	
	return 0;
}
