SRC=src/main.cpp src/components/audio/audio.cpp

all:
	g++ -std=c++14 ${SRC} -O2 -o bin/prog -lsfml-audio -lsfml-system
run:
	./bin/prog
make:
	make && make run