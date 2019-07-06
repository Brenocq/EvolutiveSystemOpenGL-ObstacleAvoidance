#include <GL/glut.h>
#include <iostream>
#include "Classes/robot.h"
#include <math.h>
using namespace std;

#define WindowHeight 800
#define WindowWidth 800
#define ScreenHeight 1080
#define ScreenWidth 1920
#define NumRobots 100

Robot robot[NumRobots];

void draw();
void timer(int);
void newPopulation();
void updatePositions(float seconds);
float distance(float x1, float y1, float x2, float y2);

int main(int argc, char** argv)
{
  srand(time(0));

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(WindowWidth, WindowHeight);
  glutInitWindowPosition((ScreenWidth/2)-(WindowWidth/2), (ScreenHeight/2)-(WindowHeight/2));
  glutCreateWindow("Obstable Avoidance Simulation");

  newPopulation();
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glutDisplayFunc(draw);
  glutPostRedisplay();
  glutTimerFunc(0, timer, 0);
  glutMainLoop();

  return 0;
}

void draw(){
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  for (int i = 0; i < NumRobots; i++) {
    robot[i].draw();
  }

  glutSwapBuffers();
}

void timer(int){
  updatePositions(10);// Update every 100ms
  glutPostRedisplay();
  glutTimerFunc(10, timer, 0);
}

void newPopulation(){
  float posX, posY, angle;

  for (int i = 0; i < NumRobots; i++) {
    posX = (rand()%100 - 50)/5.0;// Some value between -10 and +10
    posY = (rand()%100 - 50)/5.0;// Some value between -10 and +10
    angle = rand()%360;
    robot[i].newOrientation(posX, posY, angle);
  }
}

void updatePositions(float seconds){
  bool canMove;
  for (int i = 0; i < NumRobots; i++) {
    canMove = true;
    for (int j = 0; j < NumRobots; j++) {
      if(j!=i){
        if(distance( robot[i].getX(),robot[i].getY(),robot[j].getX(),robot[j].getY() ) < robot[i].getRadius()+robot[j].getRadius()){
          canMove = false;
          robot[i].newColor(1,0,0);
          robot[i].
        }
      }
    }
    if(canMove){
      robot[i].newColor(0,0,0);
      robot[i].move(0.01, 0, seconds);
    }
  }
}

float distance(float x1, float y1, float x2, float y2){
  return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}
