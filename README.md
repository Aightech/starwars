

>```
>
.______        ______   .______      ____    __    ____  ___      .______          _______.
|   _  \      /  __  \  |   _  \     \   \  /  \  /   / /   \     |   _  \        /       |
|  |_)  |    |  |  |  | |  |_)  |     \   \/    \/   / /  ^  \    |  |_)  |      |   (----`
|      /     |  |  |  | |   _  <       \            / /  /_\  \   |      /        \   \    
|  |\  \----.|  `--'  | |  |_)  |       \    /\    / /  _____  \  |  |\  \----.----)   |   
| _| `._____| \______/  |______/         \__/  \__/ /__/     \__\ | _| `._____|_______/    
>                                                                                           
>                                                                                                          
> ```                              - REVENGE OF THE DROIDS -

# starwars

Created by Alexis DEVILLARD and Maëva ARLANDIS
ROB 4 @ Polytech Sorbonne
06/2018

// STORY

Rebel Alliance or Galactic Empire ? Choose your side and try to stop your opponent from building 
its new battleship with your army of droids !

ROB WARS is a 2-player game which can be played online or offline.
Every turn, you can develop your basement and enhance your army of droids by buildings warehouses, farms
and towers but be carreful, your amount of thorilide is limited !

You win if you manage to destroy the headquarter of your opponent and lose if yours is destroyed.

// HOW TO PLAY

When it is your turn, you can build new buildings by clicking on the buttons bellow the map and then on the 
map where you want to place it. 
You can choose the target of one or several units at the same time by clicking or click-dragging across then on 
the map and then clicking at the place you want them to go.
Units and towers attack automatically when opponents are around them.

In offline mode click on the "Next" button to fnish your turn.


Units are created automatically at the end of the turn if the player has free population space.


// DESCRIPTION

- Headquarter: 
	HP -> 1500
	description: each player has one headquarter, the first one who has its headquarter destroyed lose the game 
	and the other win

- Unit: 
	HP -> 100
	DEFENSE -> 5
	description: are created automatically by warehouses if the player has free population space
	BB-8 droid for rebel alliance and BB-9E for Galactic Empire 

- Buildings:

	- Farm:
	HP -> 999
	description: add 5 to the maximum amount of population 

	- Tower:
	HP -> 200
	description: attack units of the opponent around itself

	- Warehouse:
	HP -> 500
	description: automatically creates units around it

// MORE INFORMATIONS

Change the macro.txt file to change HP, Defense and other values.

// UPGRADES

- Add SuperUnit (inherit from Element) with a special capacity / which can do more damages
- Add ressources management, cost to buildings, amount of ressources generate each turn
- Add text on the game interface for each player (number of ressources and robots)
- One player mode with a bot to play the 2nd player

// GAME STRUCTURE

Game created:
___NetworkInterface created: 
______ -listen GameServer requests (thread)
______ -send GameServer requests
______ -read and write to Rx et Tx Buffers
___GUI created:
______ (thread)
______ - read Game data (players/units/building)
______ - write and display the window
______ - get user events
______ - write Game data (players/units/building)


***Menu selection***
___play
______solo
______Lan
_______*Show current hosted game(name/nbPlayers:nbMaxPlayer/IP/port) + pseudo textbox*
_________host
__________*create server process(port)the player 1 is the host* 
_________join
__________*Show creation game menu*
__________*Options : GameName/Team(color/jedi/sith)/map/players joining the game/nbMaxPlayer*
__________*Launch game*
___option //default options are read in a text file
____*Show Option game menu*
____*Options : Port/Pseudo/ServerPort/...
___quit

***Server***


***Launch game***

___if(new message server)
______get listactionTx and map

___update list units and map = int[L*l][2]: ID and Life
___send liste:actionTx[nbMaxElement] = [(int) type |(double) value ] 
               
***update list***
___for element in listeElement
______element.update(*actionRx,*actionRx,*map);
_________process(actionRx[ID%10000])
_________action(*actionTx,*map):move/contruct/attack/heal...






