#include <iostream>
#include <string>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "src/gamelib.hpp"

#include <time.h>


int main(int argc,char ** argv)
{
	srand(time(NULL));
	cout << "hey"<< endl;

	Game game;
	game.verbose();
	game.setConnectable();

	game.setServer(2000);

	//game.startGUI();
	char enter[1024];
	int n;
	

	while(1)
	{//update game
		if(game.getReceiverBuffer(enter)>-1)
		{
			cout << "buffer n°"<< n<< ": "<<enter<< endl;
			game.processPlayerRequest(enter);
		}
		game.update();
	}

	cout << "Game ended" <<endl;

	return 0;
}
