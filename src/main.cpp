#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "Classes/robot.h"
#include "Classes/utils.h"

using namespace std;

#define WindowHeight 800
#define WindowWidth 800
#define ScreenHeight 1080
#define ScreenWidth 1920
#define NumRobots 50

vector<Robot> robot(NumRobots);

void draw();
void timer(int);
void newPopulation();
void updatePositions(float seconds);
float distance(Robot *r1, Robot *r2);
float angleTwoPoints(Robot *r1, Robot *r2);
float distTwoAngles(float a1, float a2);

int main(int argc, char** argv){
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
  updatePositions(0.020);// Update every 20ms


  glutPostRedisplay();
  glutTimerFunc(20, timer, 0);
}

void newPopulation(){
  float posX, posY, angle, genes[6];

  for (int i = 0; i < NumRobots; i++){
    robot[i].setId(i);

    posX = (rand()%100 - 50)/5.0;// Some value between -10 and +10
    posY = (rand()%100 - 50)/5.0;// Some value between -10 and +10
    angle = rand()%360;

    // Random position until reach a valid Position
    bool validposition=false;
    while(!validposition){
      validposition=true;
      robot[i].newOrientation(posX, posY, angle);
      for (int j = 0; j < NumRobots; j++){
        if(i!=j){
          if(distanceTwoRobots( &robot[i],&robot[j] ) <= (robot[i].getRadius()+robot[j].getRadius())){
            validposition=false;
            posX = (rand()%100 - 50)/5.0;// Some value between -10 and +10
            posY = (rand()%100 - 50)/5.0;// Some value between -10 and +10
          }
        }
      }
    }

    // SideSensorActivation   (0-3)meters
    genes[0] = 2;//(rand()%300)/100.0;
    // FrontSensorActivation  (0-3)meters
    genes[1] = 2;//(rand()%300)/100.0;
    // LinearVelocity         (0-1)meters/second
    genes[2] = 2;//(rand()%100)/100.0;
    // MaximumRotation        (0-10)degrees
    genes[3] = (rand()%1000)/100.0;
    // SensorAngle            (0-90)degrees
    genes[4] = (rand()%9000)/100.0;
    // Set random genes
    robot[i].newGene(genes);

  }
}

void updatePositions(float seconds){
  for (int i = 0; i < NumRobots; i++) {
    robot[i].move(robot, seconds);
  }
  /*bool canMove;
  for (int i = 0; i < NumRobots; i++) {
    canMove = true;
    //cout<<"----"<<i<<"----"<<endl;
    for (int j = 0; j < NumRobots; j++) {
      if(j!=i){
        if(distance( &robot[i],&robot[j] ) <= (robot[i].getRadius()+robot[j].getRadius())){
          canMove = false;

          robot[i].newColor(1,0,0);
          if(distTwoAngles( robot[i].getTheta(), angleTwoPoints(&robot[j],&robot[i]))<=90 ||
            distTwoAngles( robot[i].getTheta(), angleTwoPoints(&robot[j],&robot[i]))>=270){
            robot[i].rotate();
            break;
          }else{
            canMove = true;
          }
        }
      }
    }
    if(canMove){
      robot[i].newColor(0,0,0);
      robot[i].move(0.01, 0, seconds);
    }
  }*/
}
