# program: test.cpp
# 	g++ test.cpp -std=c++17 -Wall -o test

# go: program
# 	./test

program: sandbox.cpp
	g++ sandbox.cpp Screen.cpp Swarm.cpp Particle.cpp -lSDL2 -std=c++17 -Wall -o sandbox

go: program
	./sandbox