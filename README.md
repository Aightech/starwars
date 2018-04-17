# starwars

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






