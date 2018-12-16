SRC1=src/main.cpp src/components/audio/audio.cpp
SRC2=src/components/network/server.cpp
SRC3=src/components/network/client.cpp
one:
	g++ -std=c++17 ${SRC1} -O2 -o bin/prog -lsfml-audio -lsfml-system
two:
	g++ -std=c++17 ${SRC2} -O2 -o bin/prog -lboost_system -lpthread
three:
	g++ -std=c++17 ${SRC3} -O2 -o bin/prog -lboost_system -lpthread
run:
	./bin/prog
make1:
	make one && make run
make2:
	make two && make run
make3:
	make three && make run