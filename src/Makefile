flags = -lglut -lGLU -lGL -march=native -funroll-loops -Wall -Wextra -std=c++11 -fopenmp
CXX=g++
CXXFLAGS=-g
all: build

run: build
	./run

build: utils.o object.o quadtree.o robot.o obstacle.o environment.o manager.o main.o

main.o: main.cpp Classes/parameters.h
	@g++ -g main.cpp -Wall -o run  $(flags) $(CXXFLAGS) utils.o robot.o obstacle.o object.o environment.o manager.o quadtree.o

object.o: Classes/object.cpp
	@g++ -g Classes/object.cpp -c $(flags) $(CXXFLAGS)

robot.o: Classes/robot.cpp Classes/parameters.h
	@g++ -g Classes/robot.cpp  -c $(flags) $(CXXFLAGS)

obstacle.o: Classes/obstacle.cpp
	@g++ -g Classes/obstacle.cpp -c $(flags) $(CXXFLAGS)

utils.o: Classes/utils.cpp
	@g++ -g Classes/utils.cpp -c $(flags) $(CXXFLAGS)

environment.o: Classes/environment.cpp Classes/parameters.h
	@g++ -g Classes/environment.cpp -c $(flags) $(CXXFLAGS)

manager.o: Classes/manager.cpp Classes/parameters.h
	@g++ -g Classes/manager.cpp -c $(flags) $(CXXFLAGS)

quadtree.o: Classes/quadtree.cpp Classes/quadtree.h
	@g++ -g Classes/quadtree.cpp -c $(flags) $(CXXFLAGS)

clean:
	@rm run *.o
