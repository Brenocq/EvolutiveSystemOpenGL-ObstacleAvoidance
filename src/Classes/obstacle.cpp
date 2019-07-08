#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include "object.h"
#include "obstacle.h"
#include "utils.h"

using namespace std;

Obstacle::Obstacle()
  :Object()
{
  color[0] = 0.4;
  color[1] = 0.4;
  color[2] = 0.4;
  radius = 0.3;
  id = -1;
}

Obstacle::~Obstacle(){
}

void Obstacle::draw(void) const{
  glColor3f(color[0], color[1], color[2]);
  glBegin(GL_POLYGON);
  for (int i = 0; i < 360; i+=10) {
    glVertex2d( radius/10*cos(i/180.0*M_PI) + x/10, radius/10*sin(i/180.0*M_PI) + y/10);
  }
  glEnd();
}

void Obstacle::setPosition(float _x, float _y){
  x = _x;
  y = _y;
}
