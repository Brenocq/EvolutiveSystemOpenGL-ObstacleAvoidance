#include "robot.h"
#include <GL/glut.h>
#include <math.h>

Robot::Robot():
  x(0), y(0), theta(0), radius(0.15)
{
  color[0] = 0;
  color[1] = 0;
  color[2] = 0;
}

Robot::~Robot(){

}

float Robot::getX() const {return x;}
float Robot::getY() const {return y;}
float Robot::getTheta() const {return theta;}
float Robot::getRadius() const {return radius;}

void Robot::draw(void) const{
  glColor3f(color[0], color[1], color[2]);
  glBegin(GL_POLYGON);
  for (int i = 0; i < 360; i++) {
    glVertex2d( radius/10*cos(i/180.0*M_PI) + x/10, radius/10*sin(i/180.0*M_PI) + y/10);
  }
  glEnd();
}

void Robot::newOrientation(float _x, float _y, float _theta){
  x = _x;
  y = _y;
  theta = _theta;
}

void Robot::newColor(float r, float g, float b){
    color[0]=r;
    color[1]=g;
    color[2]=b;
}

void Robot::move(float linearVel, float angularVel, float seconds){
  /*tetha+=anglarVel*time;
  while(tetha>255){
    tetha-=255;
  }*/
  theta+=angularVel*seconds;
  x += cos(theta/180*M_PI)*linearVel;
  y += sin(theta/180*M_PI)*linearVel;

  if(x>10)
    x=-10;
  if(x<-10)
    x=10;
  if(y>10)
    y=-10;
  if(y<-10)
    y=10;
}
