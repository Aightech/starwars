

```
>
.______        ______   .______      ____    __    ____  ___      .______          _______.
|   _  \      /  __  \  |   _  \     \   \  /  \  /   / /   \     |   _  \        /       |
|  |_)  |    |  |  |  | |  |_)  |     \   \/    \/   / /  ^  \    |  |_)  |      |   (----`
|      /     |  |  |  | |   _  <       \            / /  /_\  \   |      /        \   \    
|  |\  \----.|  `--'  | |  |_)  |       \    /\    / /  _____  \  |  |\  \----.----)   |   
| _| `._____| \______/  |______/         \__/  \__/ /__/     \__\ | _| `._____|_______/    
>                                                                                           
>                                                                                                          
>                              - REVENGE OF THE DROIDS -
``` 
# starwars

Created by Alexis DEVILLARD and Maëva ARLANDIS
ROB 4 @ Polytech Sorbonne
06/2018

//####### DESCRIPTION #######//

Rebel Alliance or Galactic Empire ? Choose your side and try to stop your opponent from building 
its new battleship with your droids !

ROB WARS is a 2-player game which can be played online or offline.
Every turn, you can develop your basement and enhance your army of droids by buildings warehouses, farms
and towers but be carreful, your amount of thorilide is limited !

You win if you manage to destroy the headquarter of your opponent and lose if yours is destroyed.

// 

* Game created:
  * NetworkInterface created: 
    * listen GameServer requests (thread)
    * send GameServer requests
    * read and write to Rx et Tx Buffers
* GUI created: (thread)
    * read Game data (players/units/building)
    * write and display the window
    * get user events
    * write Game data (players/units/building)


***Menu selection***
* play 
* lan
(*Show current hosted game(name/nbPlayers:nbMaxPlayer/IP/port) + pseudo textbox*)
  * host
  (*create server process(port)the player 1 is the host*)
  * join
  (*Show creation game menu*)
* Options : GameName/Team(color/jedi/sith)/map/players joining the game/nbMaxPlayer
(option //default options are read in a text file)
(*Show Option game menu*)
* Options : Port/Pseudo/ServerPort/...
* quit

***PLAY***
* Launch game

#MISSING
* tower implemention (so it can fire at enemies) ==> need to implement its update method and maybe create a element bullet
* debug network playing
* add Lan menu page to select founded game and cusomize game
* display the number of population and the number of thorolyde
* add a better end to the game when a player win
* implement the option menu section








