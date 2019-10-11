#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "robot.h"
#include "object.h"
#include "utils.h"

using namespace std;

Robot::Robot():
  Object(),theta(0), lastX(0), lastY(0),timeLastXY(0), inCollision(false)
{
  radius = 0.15;
  id = 0;
  color[0] = 0;
  color[1] = 0;
  color[2] = 0;
}

Robot::~Robot(){

}

float Robot::getTheta() const {return theta;}

void Robot::newGenes(vector<float> _genesAnatomy, vector<float> _genesBrain){
  genesAnatomy.clear();
  genesBrain.clear();
  for (auto gene :  _genesAnatomy) {
    genesAnatomy.push_back(gene);
  }
  for (auto gene :  _genesBrain) {
    genesBrain.push_back(gene);
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
  int qtdIR = genesAnatomy[2];
  int qtdSensors = qtdIR;
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

  //----- Draw IR sensors -----//
  if(sensorValues.size()>=qtdIR){
    int startIndexIR = 3;// IR sensor info first index

    int sensorValuesIndex = 0;
    for(int i=startIndexIR ; i<startIndexIR+qtdIR*2 ; i+=2){
      float angle = genesAnatomy[i];
      float maxDist = genesAnatomy[i+1];

      float sensorValue = sensorValues[sensorValuesIndex];

      glColor3f(0.8, 0, 0);
      glBegin(GL_LINES);
        glVertex2d( x/10 + (radius)/10*cos((theta+angle)/180.0*M_PI), y/10 + (radius)/10*sin((theta+angle)/180.0*M_PI));
        glVertex2d( x/10 + (radius+sensorValue)/10*cos((theta+angle)/180.0*M_PI), y/10 + (radius+sensorValue)/10*sin((theta+angle)/180.0*M_PI));
      glEnd();
      sensorValuesIndex++;
    }
  }
}

void Robot::move(vector<Object*> objects, float seconds){
  bool sensorTrigged=false;
  inCollision = false;

  float velocity=0;
  float rotation=0;


  updateSensor(objects);

  // Search for pysical contact between robots
  vector<Object*> nearObjects = objects;

  float fitnessBack = fitness.back();
  for (int i = 0; i < int(nearObjects.size()); i++) {
    if(nearObjects[i]->getId()!=objects[id]->getId()){
      if(distanceTwoObjects(objects[id], nearObjects[i]) <= (objects[id]->getRadius()+nearObjects[i]->getRadius())){
        inCollision = true;
        fitnessBack+=pointsCollision*seconds;
        if(distTwoAngles( this->getTheta(), angleTwoObjects(objects[id],nearObjects[i]))<90 ||
          distTwoAngles( this->getTheta(), angleTwoObjects(objects[id],nearObjects[i]))>270){
        }
      }
    }
  }
  fitness.back() = fitnessBack;

  //----- Calculate velocity -----//
  float maxVel = genesAnatomy[0];
  for(int i=0 ; i< int(genesBrain.size()/2) ; i++){
    velocity+=sensorValues[i]*genesBrain[i];
  }
  velocity = min(velocity, maxVel);
  //----- Calculate rotation -----//
  float maxRot = genesAnatomy[1];
  for(int i=int(genesBrain.size()/2) ; i<int(genesBrain.size()) ; i++){
    rotation+=sensorValues[i]*genesBrain[i];
  }
  rotation = ( int(rotation*100) % int(360*100) )/100.0f;
  if(rotation<=180) rotation = min(rotation, maxRot);
  else              rotation = max(rotation, 360-maxRot);

  //----- Rotate robot -----//
  cout<<"flag1 - "<<rotation<<endl;
  rotate(rotation);
  cout<<"flag2"<<endl;
  //----- Move robot -----//
  x += cos((theta+rotation)/180*M_PI)*velocity*seconds;
  y += sin((theta+rotation)/180*M_PI)*velocity*seconds;

  // Check collision with walls
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

  //----- Color robot -----//
  objects[id]->setColor(0,0,0);
  if(inCollision) objects[id]->setColor(1,0,0);

  // Calculate fitness each 2 seconds
  timeLastXY+=seconds;
  if(timeLastXY>=2){
    float displacedDistance = sqrt(pow(lastX-x,2)+pow(lastY-y,2));
    fitness.back() = fitness.back() + displacedDistance*pointsMoving;
    // Equal to genes[2]*seconds*pointsMoving when maximum is 1m/s
    lastX = x;
    lastY = y;
    timeLastXY = 0;
  }

}

void Robot::rotate(float angle){
  setTheta(theta+angle);
}

void Robot::updateSensor(vector<Object*> objects){
  int qtdIR = genesAnatomy[2];
  int qtdSensors = qtdIR;
  sensorValues.resize(qtdSensors);
  sensorValues.assign(qtdSensors, 10);// Set to a big value to change to the minimum
  //-----------------------------------//
  //----- Update infrared sensors -----//
  //-----------------------------------//
  vector<float> IRangle;
  vector<float> IRactivation;

  int startIndexIR = 3;// IR sensor info first index
  int sensorValuesIndex = 0;
  // Loop for each IR sensor
  for(int i=startIndexIR ; i<startIndexIR+qtdIR*2 ; i+=2){
    float angle = genesAnatomy[i];
    float maxDist = genesAnatomy[i+1];

    IRangle.push_back(angle+theta);
    IRactivation.push_back(maxDist);

    sensorValuesIndex++;
  }

  // Change to correct range
  for (int i = 0; i < qtdIR; i++) {
    while(IRangle[i]<0){
      IRangle[i]+=360;
    }
    while(IRangle[i]>=360){
      IRangle[i]-=360;
    }
  }

  // Check detection of robots
  for (int sensor = 0; sensor < qtdIR; sensor++) {
    vector<Object*> nearObjects = objects;//qTree->queryCircle(&objects[id], IRactivation[sensor]+maxRadius);
    for (int i = 0; i < int(nearObjects.size()); i++) {
      if(nearObjects[i]->getId()!=this->getId()){
        float distance, angle;
        float angleToReadRobot;// Angle interval read the other robot with this sensor
        // Calculation of distance
        distance = distanceTwoObjects(objects[id], nearObjects[i]);
        // Canculation of angle
        angle = angleTwoObjects(objects[id], nearObjects[i]);

        angleToReadRobot = atan2(distance,radius);
        if(angleToReadRobot<0){
          angleToReadRobot+=M_PI*2;
        }
        angleToReadRobot*=180/M_PI;// Convert from radians to degrees

        if(distance<=IRactivation[sensor]+radius+nearObjects[i]->getRadius()){
          float angleOtherObject = atan2(nearObjects[i]->getRadius(),distance)/M_PI*180;
          if(distTwoAngles(IRangle[sensor],angle)<angleOtherObject){
            sensorValues[sensor]=min(sensorValues[sensor],distance-radius-nearObjects[i]->getRadius());
          }
        }
      }
    }

    // Check detection of walls
    float aux;
    if((IRactivation[sensor]+radius)*cos(IRangle[sensor]/180*M_PI)+x>=10){
      aux = ((IRactivation[sensor]+radius)*cos(IRangle[sensor]/180*M_PI)+x-10)/(IRactivation[sensor]+radius)*cos(IRangle[sensor]/180*M_PI);
      aux = 1-fabs(aux);
      sensorValues[sensor]= min(sensorValues[sensor],IRactivation[sensor]*aux*(IRactivation[sensor]/(radius+IRactivation[sensor])));
    }
    if((IRactivation[sensor]+radius)*cos(IRangle[sensor]/180*M_PI)+x<=-10){
      aux = ((IRactivation[sensor]+radius)*cos(IRangle[sensor]/180*M_PI)+x+10)/(IRactivation[sensor]+radius)*cos(IRangle[sensor]/180*M_PI);
      aux = 1-fabs(aux);
      sensorValues[sensor]= min(sensorValues[sensor],IRactivation[sensor]*aux*(IRactivation[sensor]/(radius+IRactivation[sensor])));
    }
    if((IRactivation[sensor]+radius)*sin(IRangle[sensor]/180*M_PI)+y>=10){
      aux = ((IRactivation[sensor]+radius)*sin(IRangle[sensor]/180*M_PI)+y-10)/(IRactivation[sensor]+radius)*sin(IRangle[sensor]/180*M_PI);
      aux = 1-fabs(aux);
      sensorValues[sensor]= min(sensorValues[sensor],IRactivation[sensor]*aux*(IRactivation[sensor]/(radius+IRactivation[sensor])));
    }
    if((IRactivation[sensor]+radius)*sin(IRangle[sensor]/180*M_PI)+y<=-10){
      aux = ((IRactivation[sensor]+radius)*sin(IRangle[sensor]/180*M_PI)+y+10)/(IRactivation[sensor]+radius)*sin(IRangle[sensor]/180*M_PI);
      aux = 1-fabs(aux);
      sensorValues[sensor]= min(sensorValues[sensor],(IRactivation[sensor])*aux*(IRactivation[sensor]/(radius+IRactivation[sensor])));
    }

    if(sensorValues[sensor]==10){
      sensorValues[sensor]=IRactivation[sensor];// Set as maxDist if sensor was not trigged
    }
  }
}

void Robot::updateMeanFitness(int sizeMean){
    meanFitness.push_back(0);
    int qtdValuesMean = max(int(fitness.size()-sizeMean),0);
    int total = meanFitness.back();
    #pragma omp parallel for reduction(+:total)
    for (int j = fitness.size()-1; j >= qtdValuesMean; j--) {
      total+=fitness[j];
    }
    meanFitness.back() = total;
    meanFitness.back()/=(fitness.size()-qtdValuesMean);
}
