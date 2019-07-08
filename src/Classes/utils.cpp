#include "utils.h"
#include "object.h"
#include <math.h>

float distanceTwoObjects(Object *o1, Object *o2){
  return sqrt( pow(o1->getX()-o2->getX(),2) + pow(o1->getY()-o2->getY(),2) );
}

float angleTwoObjects(Object *o1, Object *o2){
  float rad;
  rad = atan2((o2->getY()-o1->getY()),(o2->getX()-o1->getX()));
  while(rad<0){
    rad+=M_PI*2;
  }
  while(rad>M_PI*2){
    rad-=M_PI*2;
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
