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
