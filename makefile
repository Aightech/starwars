all: game 

game: bin/main.o bin/guilib.o bin/gamelib.o bin/netlib.o bin/buttonlib.o bin/textboxlib.o
	g++ -o game bin/main.o bin/guilib.o bin/buttonlib.o bin/textboxlib.o bin/gamelib.o bin/netlib.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lpthread

bin/main.o: main.cpp src/netlib.hpp
	g++ -c main.cpp -o bin/main.o
	
bin/netlib.o: src/netlib.cpp src/netlib.hpp 
	g++ -c src/netlib.cpp -o bin/netlib.o
	
	
	
bin/guilib.o: src/GUI/guilib.cpp src/GUI/guilib.hpp src/GUI/buttonlib.hpp src/GUI/textboxlib.hpp
	g++ -c src/GUI/guilib.cpp -o bin/guilib.o
	
bin/buttonlib.o: src/GUI/buttonlib.cpp src/GUI/buttonlib.hpp
	g++ -c src/GUI/buttonlib.cpp -o bin/buttonlib.o

bin/textboxlib.o: src/GUI/textboxlib.cpp src/GUI/textboxlib.hpp
	g++ -c src/GUI/textboxlib.cpp -o bin/textboxlib.o
	
	
	
bin/gamelib.o: src/gamelib.cpp src/gamelib.hpp 
	g++ -c src/gamelib.cpp -o bin/gamelib.o
	

	
clean:
	rm -f bin/*.o
	

