#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
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
#define displaySteps 1000

// Evolutive system global parameters (fixed)
#define numRobots 10
#define numEnvironemnts 1
#define numObstacles 50
#define pointsCollision -50.0// Points per second in collision
#define pointsMoving 10.0// Points per second in maximum speed
#define qtdPopulationsTested 60// Number of populations tested with each environment
#define populationTestTime 60// Time that each population will be tested
#define qtdRepetitions 15// Number of times that each environment will be tested to define the fitness
#define envMutationRate 0.2

// Debug
#define showAvFitness 0
#define showPopulationRobots 0
#define showPopulationEnvironments 1

// Global variables
vector<Environment> environment(numEnvironemnts);
int populationNum = 0;
int populationEnvironmentNum = 0;
int environmentBeingTested = 0;// Index of the environement being tested
int populationMeanFitness;// Mean fitness of all robots
int bestRobotFitness;
char fileName[30];
ofstream outputFile;

void draw();
void timer(int);
//----- Functions to test environments -----//
void firstPopulationEnvironments();
void newPopulationEnvironments();
void setEnvironmentParameters();
//----- User Interface -----//
void generationProgressBar();
//----- Data generation -----//
void setFileName(string name);

int main(int argc, char** argv){
  srand(time(0));

  if(argc>1){
    string input(argv[1]);
    setFileName(input);
  }else{
    setFileName("temp.txt");
  }

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
  // Show robots every 10000 updates
  for(int rep=0;rep<displaySteps;rep++){
    for (size_t i = 0; i < numEnvironemnts; i++) {
      environment[0].updateRobots(0.200);// Update as 200ms
    }

    /*updatePositions(0.200);// Update as 200ms
    currTime+=0.200;

    if(currTime>=populationTestTime){
      newPopulationRobots();
      currTime=0;
      populationNum++;
      //----- Update outputFile -----//
      outputFile.open(fileName, std::ios::app);
      if(populationNum==1){
        outputFile<<"\npop " << populationNum <<" env "<<environmentBeingTested<<" rep "<< environment[environmentBeingTested].auxFitness.size()<<" : ";
      }
      outputFile << bestRobotFitness<<" ";
      outputFile.close();
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
      //newPopulationEnvironments();
      populationEnvironmentNum++;
    }*/
  }

  glutPostRedisplay();
  glutTimerFunc(1000/60, timer, 0);// Call timer function as fast as possible
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

//----- Data generation -----//
void setFileName(string name){
  string fileNameStr;
  if(name.find(".txt")!= string::npos){
    fileNameStr = "Data/"+name;
  }else{
    fileNameStr = "Data/"+name+".txt";
  }
  fileNameStr.copy(fileName, fileNameStr.size()+1);
  cout << "This simulation will be saved to: " << fileName<<endl;
  // Clean file
  outputFile.open(fileName);
  outputFile.close();
}
