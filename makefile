all:
	g++ -std=c++14 src/main.cpp -O2 -o bin/prog -lportaudio
build:
	cd portaudio/portaudio && ./configure && sudo make install
run:
	./bin/prog