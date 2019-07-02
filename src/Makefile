all: robot.o main.o

main.o: main.cpp
	@g++ main.cpp -Wall -o run -lglut -lGLU -lGL

robot.o: Classes/robot.cpp
	@g++ Classes/robot.cpp -Wall -c -lglut -lGLU -lGL

clear:
	@rm run *.o
