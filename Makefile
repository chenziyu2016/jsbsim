all:
	g++ src/mcs.cpp -o bin/mcs -Iinclude/JSBSim -Llib -lJSBSim -Wl,-rpath,./lib
clean:
	rm -f bin/mcs

