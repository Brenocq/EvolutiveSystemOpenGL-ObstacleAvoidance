#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include "Classes/robot.h"
#include "Classes/obstacle.h"
#include "Classes/utils.h"
#include "Classes/environment.h"

// TODO predator
// TODO generate file with fitness qtdValuesMean
// TODO create matlabApp to visualize data

using namespace std;

// Window Parameters
#define windowHeight 600
#define windowWidth 600
#define screenHeight 1080
#define screenWidth 1920

// Evolutive system global parameters (fixed)
#define numRobots 10
#define numEnvironemnts 5
#define numObstacles 90
#define pointsCollision -50.0// Points per second in collision
#define pointsMoving 10.0// Points per second in maximum speed
#define qtdPopulationsTested 30// Number of populations tested with each environment
#define populationTestTime 120// Time that each population will be tested
#define qtdRepetitions 5// Number of times that each environment will be tested to define the fitness
#define envMutationRate 0.2

// Environment parameters
int fitnessMean;// Number of fitness values that will be used to calculate the meanFitness
float mutationRate;// Chance of each gene suffer mutation
float neutralCrossing;// 0 -> became the best robot, 1-> do not evolve
float neutralMutation;// 0 -> change to mutation, 1-> do not mutate
bool controlBackMutationPrevention;// Control if will use back mutation prevention
int crossingCondition;// 0:fitness<fitness, 1:fitness<meanFitness, 2:meanFitness<meanFitness

// Debug
#define showAvFitness 0
#define showPopulationRobots 0
#define showPopulationEnvironments 1

// Global variables
vector<Robot> robot(numRobots);
vector<Environment> environment(numEnvironemnts);
vector<Obstacle> obstacle(numObstacles);
float currTime;
int populationNum = 0;
int populationEnvironmentNum = 0;
int environmentBeingTested = 0;// Index of the environement being tested
int populationMeanFitness;// Mean fitness of all robots

void draw();
void timer(int);
//----- Functions to test robots -----//
void firstPopulationRobots();
void newPopulationRobots();
void randomPositions();
void updatePositions(float seconds);
//----- Functions to test environments -----//
void firstPopulationEnvironments();
void newPopulationEnvironments();
void setEnvironmentParameters();

void generationProgressBar();

int main(int argc, char** argv){
  srand(time(0));
  //----- Create window -----//
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(windowWidth, windowHeight);
  glutInitWindowPosition((screenWidth/2)-(windowWidth/2), (screenHeight/2)-(windowHeight/2));
  glutCreateWindow("Obstacle Avoidance Simulation");

  firstPopulationRobots();// Create the first population of robots
  firstPopulationEnvironments();// Create the first population of environments
  setEnvironmentParameters();
  glClearColor(1.0, 1.0, 1.0, 1.0);// Clear window
  glutDisplayFunc(draw);// Set the draw function

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
  generationProgressBar();
  glutSwapBuffers();
}

void timer(int){
  // Show robots every 1000 updates
  for(int rep=0;rep<1000;rep++){
    updatePositions(0.200);// Update as 200ms
    currTime+=0.200;

    if(currTime>=populationTestTime){
      newPopulationRobots();
      currTime=0;
      populationNum++;
    }
    //----- Tested one repetition in the environment -----//
    if(populationNum == qtdPopulationsTested){
      populationNum = 0;
      environment[environmentBeingTested].auxFitness.push_back(populationMeanFitness);
      if(showPopulationEnvironments){
        cout<<"Environment ("<<environmentBeingTested<<"/"<<numEnvironemnts-1<<") - Repetition ("<<
        environment[environmentBeingTested].auxFitness.size()<<"/"<<qtdRepetitions<<
        ") - Generations:"<<qtdPopulationsTested<<" - best fitness:"<<
        environment[environmentBeingTested].auxFitness.back()<<endl;
      }
    }
    //----- Tested all repetitions of a environment -----//
    if(environment[environmentBeingTested].auxFitness.size() == qtdRepetitions){
      float meanFitness=0;
      // Calculate mean (exclude best and worst value)
      sort(environment[environmentBeingTested].auxFitness.begin(),
      environment[environmentBeingTested].auxFitness.end());
      for (int i = 1; i < qtdRepetitions-1; i++) {
        meanFitness+=environment[environmentBeingTested].auxFitness[i];
      }
      meanFitness/=(qtdRepetitions-2);

      // Update environment fitness
      environment[environmentBeingTested].auxFitness.clear();
      environment[environmentBeingTested].fitness.push_back(meanFitness);

      if(showPopulationEnvironments){
        cout<<"--- Environment "<<environmentBeingTested<<" - Final fitness: "<<
        environment[environmentBeingTested].fitness.back()<<endl;
      }

      environmentBeingTested++;
      if(environmentBeingTested<numEnvironemnts){
        setEnvironmentParameters();
      }
    }
    //----- Tested all environments -----//
    if(environmentBeingTested==numEnvironemnts){
      if(showPopulationEnvironments){
        cout<<"FINISH TESTING ENVIRONMENTS (Generation "<< populationEnvironmentNum <<")"<<endl;
        for (int i = 0; i < numEnvironemnts; i++) {
          cout<<"\tEnvironment "<<i<<": "<<environment[i].fitness.back()<<endl;
          cout<<"\t\tFitness mean: "<<environment[i].genes[0]<<endl;
          cout<<"\t\tMutation rate: "<<environment[i].genes[1]<<endl;
          cout<<"\t\tNeutral crossing: "<<environment[i].genes[2]<<endl;
          cout<<"\t\tNeutral mutation: "<<environment[i].genes[3]<<endl;
          cout<<"\t\tBack mutation prevention: "<<environment[i].genes[4]<<endl;
          cout<<"\t\tCrossing condition: "<<environment[i].genes[5]<<endl;
        }
      }
      environmentBeingTested=0;
      newPopulationEnvironments();
      populationEnvironmentNum++;
    }
  }

  glutPostRedisplay();
  glutTimerFunc(1000/60, timer, 0);// Call timer function as fast as possible
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


//----- Robots -----//
void firstPopulationRobots(){
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

void newPopulationRobots(){
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
  populationMeanFitness = totalFitness/numRobots;
  if(showPopulationRobots){
    cout<<endl<<"Fitness Population "<<populationNum<<":"<<endl;
    cout<<"\tMaximum Fitness: "<<populationTestTime*pointsMoving<<endl;
    cout<<"\tMinimum Fitness: "<<populationTestTime*pointsCollision<<endl;
    cout<<"\tAverage Fitness: "<<populationMeanFitness<<endl;
    cout<<"\tRobot "<<bestRobot<<" was the best robot with mean fitness equal to "<<bestRobotFitness
        <<" and fitness equal to "<<robot[bestRobot].fitness.back()<<endl<<endl;
  }
  //----- Crossing -----//
  bool condition;
  for (int i = 0; i < numRobots; i++) {

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

//----- Environment -----//
void firstPopulationEnvironments(){
  for (int i = 0; i < numEnvironemnts; i++){
    // fitnessMean   (0.0-10.0)qtdRobots
    environment[i].genes[0] = (rand()%1000)/100.0;
    // mutationRate  (0.0-1.0)
    environment[i].genes[1] = (rand()%100)/100.0;
    // neutralCrossing (0.0-1.0)
    environment[i].genes[2] = (rand()%100)/100.0;
    // neutralMutation (0.0-1.0)
    environment[i].genes[3] = (rand()%100)/100.0;
    // controlBackMutationPrevention (0.0-1.0) (<0.5)(>=0.5)
    environment[i].genes[4] = (rand()%100)/100.0;
    // crossingCondition  (0.0-3.0) (<1)(<2)(<3)
    environment[i].genes[5] = (rand()%300)/100.0;

    environment[i].fitness.push_back(0);
  }
}

void newPopulationEnvironments(){
  vector< pair<int,int> >bestEnvironments;
  int bestEnvironment;

  for (int i = 0; i < numEnvironemnts; i++){
    bestEnvironments.push_back(make_pair(environment[i].fitness.back(),i));
  }
  sort(bestEnvironments.rbegin(),bestEnvironments.rend());

  bestEnvironment = bestEnvironments[0].second;
  for (int i = 0; i < numEnvironemnts; i++) {
    for (int j = 0; j < int(environment[0].genes.size()); j++) {
      environment[i].genes[j] = (environment[i].genes[j] + environment[bestEnvironment].genes[j])/2;
    }
  }

  if(showPopulationEnvironments){
    cout<<"Best environment:"<<bestEnvironment<<endl;
  }

  //----- Mutation -----//
  for (int i = 0; i < numEnvironemnts; i++) {
    if(environment[i].fitness.back()<environment[bestEnvironment].fitness.back()){
      for (int j = 0; j < int(environment[0].genes.size()); j++) {
      int chanceMutation = rand()%100;
        if(chanceMutation < envMutationRate*100){
          switch(j){
            case 0:
            environment[i].genes[0] = (rand()%1000)/100.0;
            break;
            case 1:
            environment[i].genes[1] = (rand()%100)/100.0;
            break;
            case 2:
            environment[i].genes[2] = (rand()%100)/100.0;
            break;
            case 3:
            environment[i].genes[3] = (rand()%100)/100.0;
            break;
            case 4:
            environment[i].genes[4] =  (rand()%100)/100.0;
            break;
            case 5:
            environment[i].genes[5] = (rand()%300)/100.0;
            break;
          }
        }
      }
    }
  }
}

void setEnvironmentParameters(){
  int i = environmentBeingTested;

  fitnessMean = int(environment[i].genes[0]);
  mutationRate = environment[i].genes[1];
  neutralCrossing = environment[i].genes[2];
  neutralMutation = environment[i].genes[3];
  controlBackMutationPrevention = (environment[i].genes[4] >= 0.5);
  crossingCondition = int(environment[i].genes[5]);
}

//----- User Interface -----//
void generationProgressBar(){
  // Rrectangle
  glColor3f(1, 0, 0);
  glBegin(GL_POLYGON);
  glVertex2d(-1,1);
  glVertex2d((float(populationNum)/qtdPopulationsTested+currTime/(qtdPopulationsTested*populationTestTime))*2-1,1);
  glVertex2d((float(populationNum)/qtdPopulationsTested+currTime/(qtdPopulationsTested*populationTestTime))*2-1,0.99);
  glVertex2d(-1,0.99);
  glEnd();
}
