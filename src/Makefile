all: robot.o utils.o main.o

main.o: main.cpp
	@g++ main.cpp -Wall -o run -lglut -lGLU -lGL robot.o utils.o

robot.o: Classes/robot.cpp
	@g++ Classes/robot.cpp -Wall -c -lglut -lGLU -lGL utils.o

utils.o: Classes/utils.cpp
	@g++ Classes/utils.cpp -Wall -c -lglut -lGLU -lGL robot.o

clear:
	@rm run *.o
