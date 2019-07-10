all: utils.o object.o robot.o obstacle.o environment.o main.o

main.o: main.cpp
	@g++ main.cpp -Wall -o run -lglut -lGLU -lGL utils.o robot.o obstacle.o object.o environment.o

object.o: Classes/object.cpp
	@g++ Classes/object.cpp -Wall -c -lglut -lGLU -lGL utils.o

robot.o: Classes/robot.cpp
	@g++ Classes/robot.cpp -Wall -c -lglut -lGLU -lGL utils.o object.o

obstacle.o: Classes/obstacle.cpp
	@g++ Classes/obstacle.cpp -Wall -c -lglut -lGLU -lGL utils.o object.o

utils.o: Classes/utils.cpp
	@g++ Classes/utils.cpp -Wall -c -lglut -lGLU -lGL object.o

environment.o: Classes/environment.cpp
	@g++ Classes/environment.cpp -Wall -c -lglut -lGLU -lGL

clear:
	@rm run *.o
