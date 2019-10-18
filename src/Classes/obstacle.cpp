#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include "object.h"
#include "obstacle.h"
#include "utils.h"

using namespace std;

Obstacle::Obstacle()
  :Object()
{
  color[0] = 0;
  color[1] = 1;
  color[2] = 0.5;
  radius = (float)(std::rand() % 10 + 1) / 11;
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
