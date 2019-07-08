#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include "Classes/robot.h"
#include "Classes/obstacle.h"
#include "Classes/utils.h"

using namespace std;

// Window Parameters
#define windowHeight 800
#define windowWidth 800
#define screenHeight 1080
#define screenWidth 1920

// Evolutive System Parameters
#define numRobots 10
#define populationTestTime 300
#define pointsCollision    -50.0// Points per second in collision
#define pointsMoving       10.0// Points per second in maximum speed
#define fitnessMean        5   // Number of fitness that will be used to calculate the mean
#define mutationRate       0.1 // Number of fitness that will be used to calculate the mean
#define neutralCrossing    0.5 // 0 -> became the best robot, 1-> do not evolve
#define neutralMutation    0.5 // 0 -> change to mutation, 1-> do not mutate
#define controlBackMutationPrevention 1 // Control if will use back mutation prevention
#define crossingCondition 0  // 0:fitness<fitness, 1:fitness<meanFitness, 2:meanFitness<meanFitness
#define numObstacles 90

// Debug
#define showAvFitness 1

vector<Robot> robot(numRobots);
vector<Obstacle> obstacle(numObstacles);
float currTime;

void draw();
void timer(int);
void firstPopulation();
void newPopulation();
void randomPositions();

void updatePositions(float seconds);

int main(int argc, char** argv){
  srand(time(0));
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(windowWidth, windowHeight);
  glutInitWindowPosition((screenWidth/2)-(windowWidth/2), (screenHeight/2)-(windowHeight/2));
  glutCreateWindow("Obstacle Avoidance Simulation");

  firstPopulation();
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glutDisplayFunc(draw);
  glutPostRedisplay();
  glutTimerFunc(0, timer, 0);
  glutMainLoop();

  return 0;
}

void draw(){
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  for (int i = 0; i < numRobots; i++) {
    robot[i].draw();
  }
  for (int i = 0; i < numObstacles; i++) {
    obstacle[i].draw();
  }

  glutSwapBuffers();
}

void timer(int){
  updatePositions(0.100);// Update as 100ms
  currTime+=0.100;
  //cout<<currTime<<endl;
  if(currTime>=populationTestTime){
    newPopulation();
    currTime=0;
  }

  glutPostRedisplay();
  glutTimerFunc(1, timer, 0);// Update every 1ms
}

void firstPopulation(){
  float genes[6];
  for (int i = 0; i < numRobots; i++){
    // SideSensorActivation   (0-3)meters
    genes[0] = (rand()%300)/100.0;
    // FrontSensorActivation  (0-3)meters
    genes[1] = (rand()%300)/100.0;
    // LinearVelocity         (0-1)meters/second
    genes[2] = (rand()%100)/100.0;
    // MaximumRotation        (0-10)degrees
    genes[3] = (rand()%1000)/100.0;
    // SensorAngle            (0-90)degrees
    genes[4] = (rand()%9000)/100.0;
    // Set random genes
    robot[i].newGene(genes);
    robot[i].setPoints(pointsCollision, pointsMoving);
    robot[i].fitness.push_back(0);
  }
  randomPositions();
}

void randomPositions(){
  float posX, posY, angle;

  //----- Random positions obstacles -----//
  for (int i = 0; i < numObstacles; i++) {
    posX = (rand()%100 - 50)/5.0;// Some value between -10 and +10
    posY = (rand()%100 - 50)/5.0;// Some value between -10 and +10
    obstacle[i].setPosition(posX, posY);
  }

  //----- Random positions robots -----//
  for (int i = 0; i < numRobots; i++){
    robot[i].setId(i);

    posX = (rand()%180 - 90)/10.0;// Some value between -9 and +9
    posY = (rand()%180 - 90)/10.0;// Some value between -9 and +9
    angle = rand()%360;
    // Random position until reach a valid Position for a robot
    bool validposition=false;
    while(!validposition){
      validposition=true;
      robot[i].newOrientation(posX, posY, angle);
      for (int j = 0; j < numRobots; j++){
        if(i!=j){
          if(distanceTwoObjects( &robot[i],&robot[j] ) <= (robot[i].getRadius()+robot[j].getRadius())){
            validposition=false;
            posX = (rand()%100 - 50)/5.0;// Some value between -10 and +10
            posY = (rand()%100 - 50)/5.0;// Some value between -10 and +10
          }
        }
      }
      for (int j = 0; j < numObstacles; j++) {
        if(distanceTwoObjects( &robot[i],&obstacle[j] ) <= (robot[i].getRadius()+obstacle[j].getRadius())){
          validposition=false;
          posX = (rand()%100 - 50)/5.0;// Some value between -10 and +10
          posY = (rand()%100 - 50)/5.0;// Some value between -10 and +10
        }
      }
    }
  }
}

void updatePositions(float seconds){
  vector<Object>objects;
  for (int i = 0; i < numRobots; i++) {
    objects.push_back(robot[i]);
  }
  for (int i = 0; i < numObstacles; i++) {
    objects.push_back(obstacle[i]);
  }
  for (int i = 0; i < numRobots; i++) {
    robot[i].move(objects, seconds);
  }
}

void newPopulation(){
  static int populationNum=1;
  int bestRobot, bestRobotFitness=0, totalFitness=0;
  vector< pair <int,int> >bestRobots;//first:fitness second:robotNumber
  vector<float>averageFitness;

  //----- Calculate Average Fitness for each Robot -----//

  for(int i = 0; i < numRobots; i++){
    if(showAvFitness){ cout<<"Robot "<<i<<": "; }
    averageFitness.push_back(0);
    int qtdValuesMean = max(int(robot[i].fitness.size()-fitnessMean),0);
    for (int j = robot[i].fitness.size()-1; j >= qtdValuesMean; j--) {
      if(showAvFitness){ cout<<robot[i].fitness[j]<<" "; }
      averageFitness.back()+=robot[i].fitness[j];
    }
    averageFitness.back()/=(robot[i].fitness.size()-qtdValuesMean);
    if(showAvFitness){ cout<<"("<<averageFitness.back()<<")"<<endl; }
  }
  //----- Select Best Robot -----//
  for (int i = 0; i < numRobots; i++){
    bestRobots.push_back(make_pair(averageFitness[i],i));
    totalFitness+=robot[i].fitness.back();
  }
  sort(bestRobots.rbegin(),bestRobots.rend());

  bestRobot = bestRobots[0].second;
  bestRobotFitness = bestRobots[0].first;
  //----- Print population data -----//
  cout<<endl<<"Fitness Population "<<populationNum<<":"<<endl;
  cout<<"\tMaximum Fitness: "<<populationTestTime*pointsMoving<<endl;
  cout<<"\tMinimum Fitness: "<<populationTestTime*pointsCollision<<endl;
  cout<<"\tAverage Fitness: "<<totalFitness/numRobots<<endl;
  cout<<"\tRobot "<<bestRobot<<" was the best robot with mean fitness equal to "<<bestRobotFitness
      <<" and fitness equal to "<<robot[bestRobot].fitness.back()<<endl<<endl;
  populationNum++;
  //----- Crossing -----//
  bool condition;
  for (int i = 0; i < numRobots; i++) {

    // TODO: Test diffenrence fitness<fitness, fitness<meanFitness, meanFitness<meanFitness
    // Select how to cross
    switch(crossingCondition){
      case 0:
        condition = robot[i].fitness.back() < robot[bestRobot].fitness.back();
      break;
      case 1:
        condition = robot[i].fitness.back() < averageFitness[bestRobot];
      break;
      case 2:
        condition = averageFitness[i] < averageFitness[bestRobot];
      break;
    }

    if(condition){
      for (int j = 0; j < int(robot[0].genes.size()); j++) {
        robot[i].genes[j] = float(neutralCrossing)*robot[i].genes[j]+(1-float(neutralCrossing))*robot[bestRobot].genes[j];
      }
    }
  }
  //----- Mutation -----//
  for (int i = 0; i < numRobots; i++) {
    robot[i].setColor(0,0,0);

    switch(crossingCondition){
      case 0:
        condition = robot[i].fitness.back() < robot[bestRobot].fitness.back();
      break;
      case 1:
        condition = robot[i].fitness.back() < averageFitness[bestRobot];
      break;
      case 2:
        condition = averageFitness[i] < averageFitness[bestRobot];
      break;
    }
    if(condition){
      // If will not use BackMutationPrevention
      int sumBMP=0;
      for (int j = 0; j < int(robot[0].mutatedGenes.size()); j++) {
        sumBMP+=robot[i].mutatedGenes[j];
      }
      if(!controlBackMutationPrevention || sumBMP==int(robot[0].mutatedGenes.size())){
        for (int j = 0; j < int(robot[0].mutatedGenes.size()); j++) {
          robot[i].mutatedGenes[j]=0;
        }
      }
      // Create new cromossome
      vector<float>genes(robot[0].genes.size());
      for (int j = 0; j < int(genes.size()); j++) {
        genes[j] = robot[i].genes[j];
      }

      for (int j = 0; j < int(genes.size()); j++) {
      int chanceMutation = rand()%100;
        if(chanceMutation < mutationRate*100 && robot[i].mutatedGenes[j]==0){
          robot[i].setColor(0.5,0.5,0);
          robot[i].mutatedGenes[j] = 1;
          switch(j){
            case 0:
            // SideSensorActivation   (0-3)meters
            genes[0] = float(neutralMutation)*genes[0] + (1-float(neutralMutation))*(rand()%300)/100.0;
            break;
            case 1:
            // FrontSensorActivation  (0-3)meters
            genes[1] = float(neutralMutation)*genes[1] + (1-float(neutralMutation))*(rand()%300)/100.0;
            break;
            case 2:
            // LinearVelocity         (0-1)meters/second
            genes[2] = float(neutralMutation)*genes[2] + (1-float(neutralMutation))*(rand()%100)/100.0;
            break;
            case 3:
            // MaximumRotation        (0-10)degrees
            genes[3] = float(neutralMutation)*genes[3] + (1-float(neutralMutation))*(rand()%1000)/100.0;
            break;
            case 4:
            // SensorAngle            (0-90)degrees
            genes[4] = float(neutralMutation)*genes[4] + (1-float(neutralMutation))*(rand()%9000)/100.0;
            break;
          }
        }
      }
      robot[i].newGene(genes);
    }
  }
  robot[bestRobot].setColor(0,0,1);

  //----- New population fitness -----//
  for (int i = 0; i < numRobots; i++) {
    robot[i].fitness.push_back(0);
  }
  //----- New population positions -----//
  randomPositions();
}
