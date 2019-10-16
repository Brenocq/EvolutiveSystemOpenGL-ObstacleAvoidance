#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "robot.h"
#include "object.h"
#include "utils.h"

using namespace std;

Robot::Robot():
  Object(), theta(0), lastX(0), lastY(0),timeLastXY(0), inCollision(false)
{
  radius = 0.15;
  id = 0;
  color[0] = 0;
  color[1] = 0;
  color[2] = 0;
  sensorValues.resize(controlQtdSensors);
  genesAnatomy.resize(controlQtdSensors);
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

  //----- Draw sensors -----//
  for(int i=2;i<int(genesAnatomy.size());i++){

    int sensorIndex = i-2;
    float angle = sensorIndex*(M_PI/float(controlQtdSensors)) + theta/180.0f*M_PI - M_PI/2;
    //cout<<i<<"("<<int(genesAnatomy[i])<<")";
    if(genesAnatomy[i]>controlEnableSensor){// If sensor is activated
      if(sensorValues[i-2]==controlSensorQtdDivisions){
        glColor3f(0.0, 0.8, 0);
      }else{
        glColor3f(0.8, 0, 0);
      }
      float metersPerDivision = float(controlSensorMaxDist)/controlSensorQtdDivisions;
      glBegin(GL_LINES);
        glVertex2d( x/10 + (radius)/10*cos(angle), y/10 + (radius)/10*sin(angle));
        glVertex2d( x/10 + (radius+sensorValues[i-2]*metersPerDivision)/10*cos(angle),
                    y/10 + (radius+sensorValues[i-2]*metersPerDivision)/10*sin(angle));
      glEnd();
    }
  }
  //cout<<endl;
}

void Robot::move(vector<Object*> objects, float seconds){
  bool sensorTrigged=false;
  inCollision = false;

  float velocity=0;
  float rotation=0;

  updateSensor(objects);

  //----- Calculate velocity -----//
  float maxVel = genesAnatomy[0];
  for(int i=0 ; i< int(genesBrain.size()/2) ; i++){
    if(genesAnatomy[i+2]>controlEnableSensor)
      velocity+=sensorValues[i]*genesBrain[i];
  }
  velocity = min(velocity, maxVel);
  //----- Calculate rotation -----//
  float maxRot = genesAnatomy[1];
  for(int i=int(genesBrain.size()/2) ; i<int(genesBrain.size()) ; i++){
    if(genesAnatomy[i-int(genesBrain.size()/2)+2]>controlEnableSensor)
      rotation+=sensorValues[i-int(genesBrain.size()/2)]*genesBrain[i];
  }

  rotation = ( int(rotation*100) % int(360*100) )/100.0f;
  if(rotation<=180) rotation = min(rotation, maxRot);
  else              rotation = max(rotation, 360-maxRot);

  //----- Rotate robot -----//
  rotate(rotation);
  //----- Move robot -----//
  x += cos((theta+rotation)/180*M_PI)*velocity*seconds;
  y += sin((theta+rotation)/180*M_PI)*velocity*seconds;

  // Search for pysical contact between robots
  vector<Object*> nearObjects = objects;

  float fitnessBack = fitness.back();
  for (int i = 0; i < int(nearObjects.size()); i++) {
    if(nearObjects[i]->getId()!=objects[id]->getId()){
      if(distanceTwoObjects(objects[id], nearObjects[i]) <= (objects[id]->getRadius()+nearObjects[i]->getRadius())){
        inCollision = true;
        fitnessBack+=pointsCollision*seconds;
      }
    }
  }
  fitness.back() = fitnessBack;

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

  if(inCollision){
    x -= cos((theta+rotation)/180*M_PI)*velocity*seconds;
    y -= sin((theta+rotation)/180*M_PI)*velocity*seconds;
  }

  //----- Color robot -----//
  objects[id]->setColor(0,0,0);
  if(inCollision) objects[id]->setColor(1,0,0);

  // Calculate fitness each 2 seconds
  timeLastXY+=seconds;
  if(timeLastXY>=5){
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
  int qtdSensors = controlQtdSensors;
  vector<float> realSensorValue;
  realSensorValue.resize(qtdSensors);
  realSensorValue.assign(qtdSensors, controlSensorMaxDist);// Set to a big value to change to the minimum
  //-----------------------------------//
  //----- Update distance sensors -----//
  //-----------------------------------//

  // Check detection of robots
  for (int sensor = 0; sensor < qtdSensors; sensor++) {
    // Only check activated sensors
    if(genesAnatomy[2+sensor]<=controlEnableSensor)
      continue;

    float sensorAngle = sensor*(180/float(controlQtdSensors)) + theta - 90;
    float sensorAngleRad = sensorAngle/180.0f*M_PI;

    vector<Object*> nearObjects = objects;//qTree->queryCircle(&objects[id], controlSensorMaxDist+maxRadius);
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

        if(distance<=controlSensorMaxDist+radius+nearObjects[i]->getRadius()){
          float angleOtherObject = atan2(nearObjects[i]->getRadius(),distance)/M_PI*180;
          if(distTwoAngles(sensorAngle,angle)<angleOtherObject){
            realSensorValue[sensor] = min(realSensorValue[sensor],distance-radius-nearObjects[i]->getRadius());
          }
        }
      }
    }

    // Check detection of walls
    float aux;
    if((controlSensorMaxDist+radius)*cos(sensorAngleRad)+x>=10){
      aux = ((controlSensorMaxDist+radius)*cos(sensorAngleRad)+x-10)/(controlSensorMaxDist+radius)*cos(sensorAngleRad);
      aux = 1-fabs(aux);
      realSensorValue[sensor]= min(realSensorValue[sensor],controlSensorMaxDist*aux*(controlSensorMaxDist/(radius+controlSensorMaxDist)));
    }
    if((controlSensorMaxDist+radius)*cos(sensorAngleRad)+x<=-10){
      aux = ((controlSensorMaxDist+radius)*cos(sensorAngleRad)+x+10)/(controlSensorMaxDist+radius)*cos(sensorAngleRad);
      aux = 1-fabs(aux);
      realSensorValue[sensor]= min(realSensorValue[sensor],controlSensorMaxDist*aux*(controlSensorMaxDist/(radius+controlSensorMaxDist)));
    }
    if((controlSensorMaxDist+radius)*sin(sensorAngleRad)+y>=10){
      aux = ((controlSensorMaxDist+radius)*sin(sensorAngleRad)+y-10)/(controlSensorMaxDist+radius)*sin(sensorAngleRad);
      aux = 1-fabs(aux);
      realSensorValue[sensor]= min(realSensorValue[sensor],controlSensorMaxDist*aux*(controlSensorMaxDist/(radius+controlSensorMaxDist)));
    }
    if((controlSensorMaxDist+radius)*sin(sensorAngleRad)+y<=-10){
      aux = ((controlSensorMaxDist+radius)*sin(sensorAngleRad)+y+10)/(controlSensorMaxDist+radius)*sin(sensorAngleRad);
      aux = 1-fabs(aux);
      realSensorValue[sensor]= min(realSensorValue[sensor],(controlSensorMaxDist)*aux*(controlSensorMaxDist/(radius+controlSensorMaxDist)));
    }
  }

  // Convert from real value to char
  float metersPerDivision = float(controlSensorMaxDist)/controlSensorQtdDivisions;
  for(int i=0; i<int(realSensorValue.size()); i++){
    sensorValues[i] = int(realSensorValue[i]/metersPerDivision);
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
