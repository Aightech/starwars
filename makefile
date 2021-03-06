all: game server

game: bin/main.o bin/guilib.o bin/animlib.o bin/gamelib.o bin/playerlib.o bin/netapi.o bin/buttonlib.o bin/textboxlib.o bin/element.o bin/warehouse.o bin/tower.o bin/farm.o bin/unit.o bin/headquarter.o
	g++ -o game bin/main.o bin/animlib.o bin/playerlib.o bin/guilib.o bin/buttonlib.o bin/textboxlib.o bin/element.o bin/unit.o bin/headquarter.o bin/warehouse.o bin/tower.o bin/farm.o bin/gamelib.o bin/netapi.o  -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lpthread -g

server: bin/server.o bin/guilib.o bin/gamelib.o bin/playerlib.o bin/netapi.o bin/buttonlib.o bin/textboxlib.o bin/element.o bin/headquarter.o bin/warehouse.o bin/tower.o bin/farm.o bin/unit.o
	g++ -o server bin/server.o bin/animlib.o bin/playerlib.o bin/guilib.o bin/buttonlib.o bin/textboxlib.o bin/element.o bin/unit.o bin/headquarter.o bin/warehouse.o bin/tower.o bin/farm.o bin/gamelib.o bin/netapi.o  -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lpthread -g


bin/server.o: server.cpp 
	g++ -c server.cpp -std=c++0x -lpthread -o bin/server.o

bin/main.o: main.cpp 
	g++ -c main.cpp -std=c++0x -lpthread -o bin/main.o

bin/netapi.o: src/network/netapi.cpp src/network/netapi.hpp 
	g++ -c src/network/netapi.cpp -std=c++0x -lpthread -o bin/netapi.o
	
	
#gui files	
bin/guilib.o: src/GUI/guilib.cpp src/GUI/guilib.hpp src/GUI/buttonlib.hpp src/GUI/textboxlib.hpp
	g++ -c src/GUI/guilib.cpp -std=c++0x -lpthread -o bin/guilib.o
	
bin/buttonlib.o: src/GUI/buttonlib.cpp src/GUI/buttonlib.hpp
	g++ -c src/GUI/buttonlib.cpp -o bin/buttonlib.o
	
bin/animlib.o: src/GUI/animlib.cpp src/GUI/animlib.hpp
	g++ -c src/GUI/animlib.cpp -o bin/animlib.o

bin/textboxlib.o: src/GUI/textboxlib.cpp src/GUI/textboxlib.hpp
	g++ -c src/GUI/textboxlib.cpp -o bin/textboxlib.o
	
#element files
bin/element.o: src/elements/element.cpp src/elements/element.hpp
	g++ -c src/elements/element.cpp -o bin/element.o -std=c++11
	
bin/headquarter.o: src/elements/element.cpp src/elements/element.hpp src/elements/building/headquarter.cpp src/elements/building/headquarter.hpp
	g++ -c src/elements/building/headquarter.cpp -o bin/headquarter.o -std=c++11

bin/warehouse.o: src/elements/element.cpp src/elements/element.hpp src/elements/building/warehouse.cpp src/elements/building/warehouse.hpp
	g++ -c src/elements/building/warehouse.cpp -o bin/warehouse.o -std=c++11

bin/tower.o: src/elements/element.cpp src/elements/element.hpp src/elements/building/tower.cpp src/elements/building/tower.hpp
	g++ -c src/elements/building/tower.cpp -o bin/tower.o -std=c++11

bin/farm.o: src/elements/element.cpp src/elements/element.hpp src/elements/building/farm.cpp src/elements/building/farm.hpp
	g++ -c src/elements/building/farm.cpp -o bin/farm.o -std=c++11
	
bin/unit.o: src/elements/element.hpp src/elements/unit/unit.cpp src/elements/unit/unit.hpp
	g++ -c src/elements/unit/unit.cpp -o bin/unit.o -std=c++11

bin/playerlib.o: src/player/playerlib.cpp src/player/playerlib.hpp 
	g++ -c src/player/playerlib.cpp -std=c++0x -lpthread -o bin/playerlib.o
	
bin/gamelib.o: src/gamelib.cpp src/gamelib.hpp 
	g++ -c src/gamelib.cpp -std=c++0x -lpthread -o bin/gamelib.o
	

	
clean:
	rm -f bin/*.o



