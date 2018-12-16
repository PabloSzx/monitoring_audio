SRC_SERVER=src/containers/server.cpp src/components/audio/audio.cpp src/components/network/server/udp.cpp
SRC_CLIENT=src/containers/client.cpp src/components/audio/audio.cpp src/components/network/client/udp.cpp
LIBRARIES=-lboost_system -lpthread -lboost_thread -lsfml-audio -lsfml-system
server:
	g++ -std=c++17 ${SRC_SERVER} -O2 -o bin/server.o ${LIBRARIES}
client:
	g++ -std=c++17 ${SRC_CLIENT} -O2 -o bin/client.o ${LIBRARIES}
run_server:
	sudo ./bin/server.o
run_client:
	./bin/client.o
make_server:
	make server && make run_server
make_client:
	make client && make run_client