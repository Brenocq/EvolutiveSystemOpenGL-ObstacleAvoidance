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
  radius = 0.2;
  id = 0;
  color[0] = 0;
  color[1] = 0;
  color[2] = 0;
  sensorValues.resize(controlQtdSensors);
  genesAnatomy.resize(2+controlQtdSensors);
  headColor.resize(3);
  headColor[0] = 1;
  headColor[1] = 1;
  headColor[2] = 1;
	immunity = 0;
}

Robot::~Robot(){

}

float Robot::getTheta() const {return theta;}
uint8_t Robot::getImmunity() const {return immunity;}
void Robot::setImmunity(uint8_t _immunity){immunity = _immunity;}
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
  //minX = maxX = x;
  //minY = maxY = y;

}

void Robot::setTheta(float _theta){
  while(_theta>=360)
    _theta-=360;
  while(_theta<0)
    _theta+=360;

  theta=_theta;
}

 void Robot::setHeadColor(int r, int g, int b){
     headColor[0]=r;
     headColor[1]=g;
     headColor[2]=b;
 }

void Robot::draw(void) const{
  // Draw robot body
  if(inCollision)
    glColor3f(1, 0, 0);
  else
    glColor3f(color[0], color[1], color[2]);
  glBegin(GL_POLYGON);
  for (int i = 0; i < 360; i+=5) {
    glVertex2d( radius/worldSize*cos(i/180.0*M_PI) + x/worldSize, radius/worldSize*sin(i/180.0*M_PI) + y/worldSize);
  }
  glEnd();

  //----- Draw front point -----//
  glColor3f(headColor[0], headColor[1], headColor[2]);
  glBegin(GL_POLYGON);
  for (int i = 0; i < 360; i+=10) {
    glVertex2d( 0.08*cos(i/180.0*M_PI)/worldSize + x/worldSize + 0.005*cos(theta/180.0*M_PI), 0.08*sin(i/180.0*M_PI)/worldSize + y/worldSize + 0.005*sin(theta/180.0*M_PI));
  }
  glEnd();

  //----- Draw sensors -----//
  for(int i=2;i<int(genesAnatomy.size());i++){
    int sensorIndex = i-2;

    // Sensor angle in radians
    float angle = theta/180.0f*M_PI + sensorIndex*(controlMaxSensorAngle/float(controlQtdSensors-1))/180.0f*M_PI - controlMaxSensorAngle/2*M_PI/180.0f;

    if(genesAnatomy[i]>controlEnableSensor){// If sensor is activated
      if(sensorValues[i-2]==controlSensorQtdDivisions){
        glColor3f(0.0, 0.8, 0);
      }else{
        glColor3f(0.8, 0, 0);
      }
      float metersPerDivision = float(controlSensorMaxDist)/controlSensorQtdDivisions;
      glBegin(GL_LINES);
        glVertex2d( x/worldSize + (radius)/worldSize*cos(angle), y/worldSize + (radius)/worldSize*sin(angle));
        glVertex2d( x/worldSize + (radius+sensorValues[i-2]*metersPerDivision)/worldSize*cos(angle),
                    y/worldSize + (radius+sensorValues[i-2]*metersPerDivision)/worldSize*sin(angle));
      glEnd();
    }
  }
}

void Robot::move(vector<Object*> objects, float seconds){
  bool wasInCollision = inCollision;
  inCollision = false;

  float velocity=0;
  float rotation=0;

  updateSensor(objects);

  float qtdActiveSensors=0;
  for (int i = 2; i < int(genesAnatomy.size()); i++) {
    if(genesAnatomy[i]>controlEnableSensor)
      qtdActiveSensors++;
  }
  qtdActiveSensors = qtdActiveSensors==0?1:qtdActiveSensors;
  //----- Calculate velocity -----//
  /*float maxVel = genesAnatomy[0];
  for(int i=0 ; i< int(genesBrain.size())/2 ; i++){
    if(genesAnatomy[i+2]>controlEnableSensor)
      velocity+=(sensorValues[i]/controlSensorQtdDivisions*genesBrain[i])/qtdActiveSensors;
  }
  if(velocity<0)
    velocity = max(velocity, -maxVel);
  else
    velocity = min(velocity, maxVel);*/

  // !!!! Constant velocity !!!!
  velocity = /*genesAnatomy[0]*/40/worldSize;
  //----- Calculate rotation -----//
  int qtdSensors = controlQtdSensors;
  float maxRot = genesAnatomy[1];
  for(int i=0 ; i<qtdSensors ; i++){
    if(genesAnatomy[i+2]>controlEnableSensor){
      if(i==2){
        rotation-=(controlSensorQtdDivisions-sensorValues[i])* /*genesBrain[i+qtdSensors]*/0.5/qtdActiveSensors;
      }else if(i==1){
        rotation-=(controlSensorQtdDivisions-sensorValues[i])* /*genesBrain[i+qtdSensors]*/4/qtdActiveSensors;
      }else{
        rotation+=(controlSensorQtdDivisions-sensorValues[i])* /*genesBrain[i+qtdSensors]*/0.5/qtdActiveSensors;
      }
    }

  }

  rotation = ( int(rotation*100) % int(360*100) )/100.0f;
  if(rotation<=180) rotation = min(rotation, maxRot);
  else              rotation = max(rotation, 360-maxRot);

  float fitnessBack = fitness.back();
  //----- Rotate robot -----//
  rotate(rotation);
  fitnessBack+=-(pointsRotation/360)*abs(rotation)*seconds;
  //----- Move robot -----//
  x += cos((theta)/180*M_PI)*velocity*seconds;
  y += sin((theta)/180*M_PI)*velocity*seconds;

  // Search for pysical contact between robots
  vector<Object*> nearObjects = objects;

  for (int i = 0; i < int(nearObjects.size()); i++) {
    if(nearObjects[i]->getId()!=objects[id]->getId()){
      if(distanceTwoObjects(objects[id], nearObjects[i]) <= (objects[id]->getRadius()+nearObjects[i]->getRadius())/* &&
      distTwoAngles(angleTwoObjects(objects[id],nearObjects[i]), theta)<90*/){
          //cout<<"Batida: "<<angleTwoObjects(objects[id],nearObjects[i])<<" Angle:"<<theta<<endl;
        inCollision = true;
      }
    }
  }

  // Check collision with walls
  if(x>worldSize-radius){
    inCollision = true;
    x=worldSize-radius;
  }
  if(y>worldSize-radius){
    inCollision = true;
    y=worldSize-radius;
  }
  if(x<-worldSize+radius){
    inCollision = true;
    x=-worldSize+radius;
  }
  if(y<-worldSize+radius){
    inCollision = true;
    y=-worldSize+radius;
  }

  // Avoid entering objects
  if(inCollision){
    x -= cos((theta+rotation)/180*M_PI)*velocity*seconds;
    y -= sin((theta+rotation)/180*M_PI)*velocity*seconds;
    rotate((1+rand()%3)*90);
  }

  // Add points for each collision
  if(inCollision==true && wasInCollision==false){
      fitnessBack+=pointsCollision*seconds;
      fitness.back() = fitnessBack;
  }

  //----- Color robot -----//
  if(inCollision) objects[id]->setColor(1,0,0);

  // Calculate fitness each 5 seconds
  timeLastXY+=seconds;
  if(timeLastXY>=5){
    float displacedDistance = sqrt(pow(lastX-x,2)+pow(lastY-y,2));
    fitness.back() = fitness.back() + displacedDistance*pointsMoving;
    // Equal to genes[2]*seconds*pointsMoving when maximum is 1m/s
    lastX = x;
    lastY = y;
    timeLastXY = 0;
  }

  // Calculate fitness with area explored
  /*minX = min(minX, x);
  maxX = max(maxX, x);
  minY = min(minY, y);
  maxY = max(maxY, y);
  fitness.back() -= lastFitnessExploration;
  float areaTravelled = (maxX-minX)*(maxX-minY);
  float fitnessExploration = areaTravelled/(worldSize*2)*(worldSize*2) * pointsExploration;
  fitness.back() += fitnessExploration;
  lastFitnessExploration = fitnessExploration;*/
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

    float sensorAngle = sensor*(controlMaxSensorAngle/float(controlQtdSensors-1)) + theta - controlMaxSensorAngle/2;
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
    if((controlSensorMaxDist+radius)*cos(sensorAngleRad)+x>=worldSize){
      aux = ((controlSensorMaxDist+radius)*cos(sensorAngleRad)+x-worldSize)/(controlSensorMaxDist+radius)*cos(sensorAngleRad);
      aux = 1-fabs(aux);
      realSensorValue[sensor]= min(realSensorValue[sensor],controlSensorMaxDist*aux*(controlSensorMaxDist/(radius+controlSensorMaxDist)));
    }
    if((controlSensorMaxDist+radius)*cos(sensorAngleRad)+x<=-worldSize){
      aux = ((controlSensorMaxDist+radius)*cos(sensorAngleRad)+x+worldSize)/(controlSensorMaxDist+radius)*cos(sensorAngleRad);
      aux = 1-fabs(aux);
      realSensorValue[sensor]= min(realSensorValue[sensor],controlSensorMaxDist*aux*(controlSensorMaxDist/(radius+controlSensorMaxDist)));
    }
    if((controlSensorMaxDist+radius)*sin(sensorAngleRad)+y>=worldSize){
      aux = ((controlSensorMaxDist+radius)*sin(sensorAngleRad)+y-worldSize)/(controlSensorMaxDist+radius)*sin(sensorAngleRad);
      aux = 1-fabs(aux);
      realSensorValue[sensor]= min(realSensorValue[sensor],controlSensorMaxDist*aux*(controlSensorMaxDist/(radius+controlSensorMaxDist)));
    }
    if((controlSensorMaxDist+radius)*sin(sensorAngleRad)+y<=-worldSize){
      aux = ((controlSensorMaxDist+radius)*sin(sensorAngleRad)+y+worldSize)/(controlSensorMaxDist+radius)*sin(sensorAngleRad);
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
    for (int j = fitness.size()-1; j >= qtdValuesMean; j--) {
      total+=fitness[j];
    }
    meanFitness.back() = total;
    meanFitness.back()/=(fitness.size()-qtdValuesMean);
}
