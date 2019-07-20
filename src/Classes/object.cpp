#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include "object.h"
#include "utils.h"

using namespace std;

Object::Object():
  x(0), y(0), radius(0.3)
{
  color[0] = 0;
  color[1] = 0;
  color[2] = 0;
}

Object::~Object(){

}

float Object::getX() const {return x;}
float Object::getY() const {return y;}
float Object::getRadius() const {return radius;}
int Object::getId() const {return id;}
void Object::setId(int _id){id=_id;}

void Object::draw(void)const{}

void Object::setColor(float r, float g, float b){
    color[0]=r;
    color[1]=g;
    color[2]=b;
}
