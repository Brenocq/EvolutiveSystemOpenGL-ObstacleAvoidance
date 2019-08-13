#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "robot.h"
#include "object.h"
#include "utils.h"

using namespace std;

Robot::Robot():
  Object(),theta(0), inCollision(false), lastX(0), lastY(0)
{
  radius = 0.15;
  id = 0;
  color[0] = 0;
  color[1] = 0;
  color[2] = 0;
  sensorValues[0] = -1;
  sensorValues[1] = -1;
  sensorValues[2] = -1;
  for (int i = 0; i < 5; i++) {
    genes.push_back(0);
    mutatedGenes.push_back(0);
  }
}

Robot::~Robot(){

}

float Robot::getTheta() const {return theta;}

void Robot::newGene(float _genes[5]){
  for (int i = 0; i < 5; i++) {
    genes[i]=_genes[i];
  }
}

void Robot::newGene(vector<float> _genes){
  for (int i = 0; i < int(_genes.size()); i++) {
    genes[i]=_genes[i];
  }
}

void Robot::newOrientation(float _x, float _y, float _theta){
  x = _x;
  lastX = x;
  y = _y;
  lastY = y;
  theta = _theta;
}


void Robot::setTheta(float _theta){
  while(_theta>=360)
    _theta-=360;
  while(_theta<0)
    _theta+=360;

  theta=_theta;
}

void Robot::draw(void) const{
  // Draw robot body
  if(inCollision)
    glColor3f(1, 0, 0);
  else
    glColor3f(color[0], color[1], color[2]);
  glBegin(GL_POLYGON);
  for (int i = 0; i < 360; i+=5) {
    glVertex2d( radius/10*cos(i/180.0*M_PI) + x/10, radius/10*sin(i/180.0*M_PI) + y/10);
  }
  glEnd();
  //----- Draw front point -----//
  glColor3f(1, 1, 1);
  glBegin(GL_POLYGON);
  for (int i = 0; i < 360; i+=10) {
    glVertex2d( 0.005*cos(i/180.0*M_PI) + x/10 + 0.005*cos(theta/180.0*M_PI), 0.005*sin(i/180.0*M_PI) + y/10 + 0.005*sin(theta/180.0*M_PI));
  }
  glEnd();
  //----- Draw left sensor lines -----//
  if(sensorValues[0]>=0){
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
      glVertex2d( x/10, y/10);
      glVertex2d( x/10 + (radius+sensorValues[0])/10*cos((theta+genes[4])/180.0*M_PI), y/10 + (radius+sensorValues[0])/10*sin((theta+genes[4])/180.0*M_PI));
    glEnd();
  } else{
    glColor3f(0, .6, 0);
    glBegin(GL_LINES);
      glVertex2d( x/10, y/10);
      glVertex2d( x/10 + (radius+genes[0])/10*cos((theta+genes[4])/180.0*M_PI), y/10 + (radius+genes[0])/10*sin((theta+genes[4])/180.0*M_PI));
    glEnd();
  }
  //----- Draw front sensor lines -----//
  if(sensorValues[1]>=0){
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
      glVertex2d( x/10, y/10);
      glVertex2d( x/10 + (radius+sensorValues[1])/10*cos(theta/180.0*M_PI), y/10 + (radius+sensorValues[1])/10*sin(theta/180.0*M_PI));
    glEnd();
  } else{
    glColor3f(0, .6, 0);
    glBegin(GL_LINES);
      glVertex2d( x/10, y/10);
      glVertex2d( x/10 + (radius+genes[1])/10*cos(theta/180.0*M_PI), y/10 + (radius+genes[1])/10*sin(theta/180.0*M_PI));
    glEnd();
  }
  //----- Draw right sensor lines -----//
  if(sensorValues[2]>=0){
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
      glVertex2d( x/10, y/10);
      glVertex2d( x/10 + (radius+sensorValues[2])/10*cos((theta-genes[4])/180.0*M_PI), y/10 + (radius+sensorValues[2])/10*sin((theta-genes[4])/180.0*M_PI));
    glEnd();
  }else{
    glColor3f(0, .6, 0);
    glBegin(GL_LINES);
      glVertex2d( x/10, y/10);
      glVertex2d( x/10 + (radius+genes[0])/10*cos((theta-genes[4])/180.0*M_PI), y/10 + (radius+genes[0])/10*sin((theta-genes[4])/180.0*M_PI));
    glEnd();
  }
}

void Robot::move(vector<Object> objects, float seconds){
  bool canMove = true;
  float angleRotation=0;
  updateSensor(objects);

  // Search for pysical contact between robots
  inCollision = false;
  for (int i = 0; i < int(objects.size()); i++) {
    if(objects[i].getId()!=objects[id].getId()){
      if(distanceTwoObjects(&objects[id], &objects[i]) <= (objects[id].getRadius()+objects[i].getRadius())){
        inCollision = true;
        fitness.back() = fitness.back()+pointsCollision*seconds;
        objects[id].setColor(1,0,0);
        if(distTwoAngles( this->getTheta(), angleTwoObjects(&objects[id],&objects[i]))<90 ||
          distTwoAngles( this->getTheta(), angleTwoObjects(&objects[id],&objects[i]))>270){
          canMove = false;
        }
      }
    }
  }

  if(inCollision){
    rotate(genes[3]);
  }
  else{

    // Calculate rotation
    if(sensorValues[1]>=0){
      canMove=false;
      angleRotation=genes[3];
    }else if(sensorValues[0]>=0 && sensorValues[2]>=0){
      angleRotation-=(sensorValues[0]/genes[0]);
      angleRotation+=(sensorValues[2]/genes[0]);
      // angleRotation will be some value between +1 and -1
      angleRotation=genes[3]*angleRotation;
    }else if(sensorValues[0]>=0){
      angleRotation=-genes[3];
    }else if(sensorValues[2]>=0){
      angleRotation=genes[3];
    }
    rotate(angleRotation);
  }

  // Move robot if possible
  if(canMove){
    x += cos(theta/180*M_PI)*genes[2]*seconds;
    y += sin(theta/180*M_PI)*genes[2]*seconds;
    if(x>10-radius){
      inCollision = true;
      x=10-radius;
    }
    if(y>10-radius){
      inCollision = true;
      y=10-radius;
    }
    if(x<-10+radius){
      inCollision = true;
      x=-10+radius;
    }
    if(y<-10+radius){
      inCollision = true;
      y=-10+radius;
    }
  }

  // Calculate fitness
  float displacedDistance = sqrt(pow(lastX-x,2)+pow(lastY-y,2));
  fitness.back() = fitness.back() + displacedDistance*pointsMoving;
  // Equal to genes[2]*seconds*pointsMoving when maximum is 1m/s
  lastX = x;
  lastY = y;
}



void Robot::rotate(float angle){
  setTheta(theta+angle);
}

void Robot::updateSensor(vector<Object> objects){
  float sensorActivaton[3] = {genes[0],genes[1],genes[0]};
  float sensorAngle[3] = {genes[4]+theta,0+theta,-genes[4]+theta};
  for (int i = 0; i < 3; i++) {
    sensorValues[i] = 10;// Set to a big value to change to the minimum
    if(sensorAngle[i]<0){
      sensorAngle[i]+=360;
    }
    if(sensorAngle[i]>=360){
      sensorAngle[i]-=360;
    }
  }
  float distance, angle;
  float angleToReadRobot;// Angle interval read the other robot with this sensor
  for (int sensor = 0; sensor < 3; sensor++) {
    for (int i = 0; i < int(objects.size()); i++) {
      if(objects[i].getId()!=this->getId()){

        // Calculation of distance
        distance = distanceTwoObjects(&objects[id], &objects[i]);
        // Canculation of angle
        angle = angleTwoObjects(&objects[id], &objects[i]);

        angleToReadRobot = atan2(distance,radius);
        if(angleToReadRobot<0){
          angleToReadRobot+=M_PI*2;
        }
        angleToReadRobot*=180/M_PI;// Convert from radians to degrees

        if(distance<=sensorActivaton[sensor]+radius+objects[i].getRadius()){
          float angleOtherObject = atan2(objects[i].getRadius(),distance)/M_PI*180;
          if(distTwoAngles(sensorAngle[sensor],angle)<angleOtherObject){
            sensorValues[sensor]=min(sensorValues[sensor],distance-radius-objects[i].getRadius());
          }
        }
      }
    }
    // Check collisions with walls
    float aux;
    if((sensorActivaton[sensor]+radius)*cos(sensorAngle[sensor]/180*M_PI)+x>=10){
      aux = ((sensorActivaton[sensor]+radius)*cos(sensorAngle[sensor]/180*M_PI)+x-10)/(sensorActivaton[sensor]+radius)*cos(sensorAngle[sensor]/180*M_PI);
      aux = 1-fabs(aux);
      sensorValues[sensor]= min(sensorValues[sensor],sensorActivaton[sensor]*aux*(sensorActivaton[sensor]/(radius+sensorActivaton[sensor])));
    }
    if((sensorActivaton[sensor]+radius)*cos(sensorAngle[sensor]/180*M_PI)+x<=-10){
      aux = ((sensorActivaton[sensor]+radius)*cos(sensorAngle[sensor]/180*M_PI)+x+10)/(sensorActivaton[sensor]+radius)*cos(sensorAngle[sensor]/180*M_PI);
      aux = 1-fabs(aux);
      sensorValues[sensor]= min(sensorValues[sensor],sensorActivaton[sensor]*aux*(sensorActivaton[sensor]/(radius+sensorActivaton[sensor])));
    }
    if((sensorActivaton[sensor]+radius)*sin(sensorAngle[sensor]/180*M_PI)+y>=10){
      aux = ((sensorActivaton[sensor]+radius)*sin(sensorAngle[sensor]/180*M_PI)+y-10)/(sensorActivaton[sensor]+radius)*sin(sensorAngle[sensor]/180*M_PI);
      aux = 1-fabs(aux);
      sensorValues[sensor]= min(sensorValues[sensor],sensorActivaton[sensor]*aux*(sensorActivaton[sensor]/(radius+sensorActivaton[sensor])));
    }
    if((sensorActivaton[sensor]+radius)*sin(sensorAngle[sensor]/180*M_PI)+y<=-10){
      aux = ((sensorActivaton[sensor]+radius)*sin(sensorAngle[sensor]/180*M_PI)+y+10)/(sensorActivaton[sensor]+radius)*sin(sensorAngle[sensor]/180*M_PI);
      aux = 1-fabs(aux);
      sensorValues[sensor]= min(sensorValues[sensor],(sensorActivaton[sensor])*aux*(sensorActivaton[sensor]/(radius+sensorActivaton[sensor])));
    }

    if(sensorValues[sensor]==10){
      sensorValues[sensor]=-1;// Set as -1 if sensor was not trigged
    }
  }
}

void Robot::updateMeanFitness(int sizeMean){
    meanFitness.push_back(0);
    int qtdValuesMean = max(int(fitness.size()-fitnessMean),0);
    for (int j = fitness.size()-1; j >= qtdValuesMean; j--) {
      meanFitness.back()+=fitness[j];
    }
    meanFitness.back()/=(fitness.size()-qtdValuesMean);

}
