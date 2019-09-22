flags = -lglut -lGLU -lGL -march=native -funroll-loops -Wall -Wextra -std=c++11 -fopenmp

all: build

build: utils.o object.o quadtree.o robot.o obstacle.o environment.o manager.o main.o

main.o: main.cpp Classes/parameters.h
	@g++ main.cpp -Wall -o run  $(flags) utils.o robot.o obstacle.o object.o environment.o manager.o quadtree.o

object.o: Classes/object.cpp
	@g++ Classes/object.cpp -c $(flags)

robot.o: Classes/robot.cpp Classes/parameters.h
	@g++ Classes/robot.cpp  -c $(flags)

obstacle.o: Classes/obstacle.cpp
	@g++ Classes/obstacle.cpp -c $(flags)

utils.o: Classes/utils.cpp
	@g++ Classes/utils.cpp -c $(flags)

environment.o: Classes/environment.cpp Classes/parameters.h
	@g++ Classes/environment.cpp -c $(flags)

manager.o: Classes/manager.cpp Classes/parameters.h
	@g++ Classes/manager.cpp -c $(flags)

quadtree.o: Classes/quadtree.cpp Classes/quadtree.h
	@g++ Classes/quadtree.cpp -c $(flags)

clean:
	@rm run *.o
