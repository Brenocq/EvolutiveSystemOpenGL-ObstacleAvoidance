#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "robot.h"
#include "utils.h"

using namespace std;

float pointsCollision = -50.0;// Points per second in collision
float pointsMoving    =  10.0;// Points per second in maximum speed

Robot::Robot():
  x(0), y(0), theta(0), radius(0.15), inCollision(false), fitness(0)
{
  id = 0;
  color[0] = 0;
  color[1] = 0;
  color[2] = 0;
  sensorValues[0] = -1;
  sensorValues[1] = -1;
  sensorValues[2] = -1;
}

Robot::~Robot(){

}

float Robot::getX() const {return x;}
float Robot::getY() const {return y;}
float Robot::getTheta() const {return theta;}
float Robot::getRadius() const {return radius;}
  int Robot::getId() const {return id;}

void Robot::newGene(float _genes[6]){
  for (int i = 0; i < 6; i++) {
    genes[i]=_genes[i];
  }
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

void Robot::setId(int _id){id=_id;}
void Robot::setTheta(float _theta){
  while(_theta>=360)
    _theta-=360;
  while(_theta<0)
    _theta+=360;

  theta=_theta;
}
void Robot::draw(void) const{
  // Draw robot body
  glColor3f(color[0], fitness/1000, color[2]);
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

void Robot::move(vector<Robot> robots, float seconds){
  bool canMove = true;
  float angleRotation=0;
  updateSensor(robots);

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

  // Search for pysical contact between robots
  for (int i = 0; i < int(robots.size()); i++) {
    if(robots[i].getId()!=robots[id].getId()){
      if(distanceTwoRobots(&robots[id], &robots[i]) <= (robots[id].getRadius()+robots[i].getRadius())){
        inCollision = true;
        fitness += pointsCollision*seconds;
        robots[id].newColor(1,0,0);
        if(distTwoAngles( robots[id].getTheta(), angleTwoRobots(&robots[id],&robots[i]))<=90 ||
          distTwoAngles( robots[id].getTheta(), angleTwoRobots(&robots[id],&robots[i]))>=270){
          rotate(genes[3]);
          canMove = false;
        }
      }
    }
  }

  // Move robot if possible
  if(canMove){
    fitness+=genes[2]*pointsMoving*seconds;
    inCollision = false;
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
  if(inCollision)
    this->newColor(1,0,0);
  else
    this->newColor(0,0,0);
}



void Robot::rotate(float angle){
  setTheta(theta+angle);
}

void Robot::updateSensor(vector<Robot> robots){
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
    for (int i = 0; i < int(robots.size()); i++) {
      if(robots[i].getId()!=this->getId()){

        // Calculation of distance
        distance = distanceTwoRobots(&robots[id], &robots[i]);
        // Canculation of angle
        angle = angleTwoRobots(&robots[id], &robots[i]);

        angleToReadRobot = atan2(distance,radius);
        if(angleToReadRobot<0){
          angleToReadRobot+=M_PI*2;
        }
        angleToReadRobot*=180/M_PI;// Convert from radians to degrees

        if(distance<=sensorActivaton[sensor]+radius+robots[i].getRadius()){
          if(distTwoAngles(sensorAngle[sensor],angle)<10){
            sensorValues[sensor]=min(sensorValues[sensor],distance-radius-robots[i].getRadius());
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
