

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

// DESCRIPTION

Rebel Alliance or Galactic Empire ? Choose your side and try to stop your opponent from building 
its new battleship with your droids !

ROB WARS is a 2-player game which can be played online or offline.
Every turn, you can develop your basement and enhance your army of droids by buildings warehouses, farms
and towers but be carreful, your amount of thorilide is limited !

You win if you manage to destroy the headquarter of your opponent and lose if yours is destroyed.

// 

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






