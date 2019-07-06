#include "utils.h"
#include "robot.h"
#include <math.h>

float distanceTwoRobots(Robot *r1, Robot *r2){
  return sqrt( pow(r1->getX()-r2->getX(),2) + pow(r1->getY()-r2->getY(),2) );
}

float angleTwoRobots(Robot *r1, Robot *r2){
  float rad;
  rad = atan2((r2->getY()-r1->getY()),(r2->getX()-r1->getX()));
  if(rad<0){
    rad+=M_PI*2;
  }
  return rad*180/M_PI;
}

float distTwoAngles(float a1, float a2){
  float distAngle = a1-a2;
  if(distAngle<=0)
    distAngle+=360;

  if(distAngle<180)
    return distAngle;
  else
    return 360-distAngle;
}
