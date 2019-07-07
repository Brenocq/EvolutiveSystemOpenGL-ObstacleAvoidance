#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "Classes/robot.h"
#include "Classes/utils.h"

using namespace std;

#define WindowHeight 800
#define WindowWidth 800
#define ScreenHeight 1080
#define ScreenWidth 1920
#define NumRobots 200
#define PopulationTime 60

vector<Robot> robot(NumRobots);
float currTime;

void draw();
void timer(int);
void firstPopulation();
void newPopulation();
void randomPositions();

void updatePositions(float seconds);
float distance(Robot *r1, Robot *r2);
float angleTwoPoints(Robot *r1, Robot *r2);
float distTwoAngles(float a1, float a2);

int main(int argc, char** argv){
  srand(time(0));

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(WindowWidth, WindowHeight);
  glutInitWindowPosition((ScreenWidth/2)-(WindowWidth/2), (ScreenHeight/2)-(WindowHeight/2));
  glutCreateWindow("Obstable Avoidance Simulation");

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

  for (int i = 0; i < NumRobots; i++) {
    robot[i].draw();
  }

  glutSwapBuffers();
}

void timer(int){
  updatePositions(0.050);// Update every 50ms

  if(currTime>=PopulationTime){
    newPopulation();
  }

  glutPostRedisplay();
  glutTimerFunc(50, timer, 0);
}

void firstPopulation(){
  float genes[6];

  for (int i = 0; i < NumRobots; i++){
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
  }
  randomPositions();
}

void randomPositions(){
  float posX, posY, angle;
  for (int i = 0; i < NumRobots; i++){
    robot[i].setId(i);

    posX = (rand()%100 - 50)/5.0;// Some value between -10 and +10
    posY = (rand()%100 - 50)/5.0;// Some value between -10 and +10
    angle = rand()%360;

    // Random position until reach a valid Position
    bool validposition=false;
    while(!validposition){
      validposition=true;
      robot[i].newOrientation(posX, posY, angle);
      for (int j = 0; j < NumRobots; j++){
        if(i!=j){
          if(distanceTwoRobots( &robot[i],&robot[j] ) <= (robot[i].getRadius()+robot[j].getRadius())){
            validposition=false;
            posX = (rand()%100 - 50)/5.0;// Some value between -10 and +10
            posY = (rand()%100 - 50)/5.0;// Some value between -10 and +10
          }
        }
      }
    }
  }
}

void updatePositions(float seconds){
  for (int i = 0; i < NumRobots; i++) {
    robot[i].move(robot, seconds);
  }
}

void newPopulation(){
  int bestRobot, bestRobotFitness=0, totalFitness=0;
  vector< pair <int,int> >bestRobots;//first:fitness second:robotNumber
  char showMutation[NumRobots][18];
  for(int i = 0; i < NumRobots; i++){
    for (size_t j = 0; j < 4; j++) {
      showMutation[i][j]=' ';
    }
  }

  //----- Calculate Average Fitness -----//
  for (size_t i = 0; i < NumRobots; i++) {
    averageFitness[i]=0;
    int aux = 0;// if some of the fitness values are -1, a change happened in all chromosomes ()
    for (size_t j = 0; j < GenMeanFit; j++) {
      if(fitness[i][j]==-1){
        aux++;
      }
      averageFitness[i]+=fitness[i][j];
    }
    if(populationNumber>=GenMeanFit){
      averageFitness[i]/=(GenMeanFit-aux);
    }else{
      averageFitness[i]/=populationNumber;
    }
  }
  //----- Select Best Robot -----//
  for (int i = 1; i <= NumRobots; i++){
    bestRobots.push_back(make_pair(fitness[i][GenMeanFit-1],i));
    totalFitness+=fitness[i][GenMeanFit-1];
  }
  sort(bestRobots.rbegin(),bestRobots.rend());

  int randNum = rand()%QtBestRobot;
  bestRobot = bestRobots[randNum].second;
  bestRobotFitness = bestRobots[randNum].first;
  //----- Print population data -----//
  if(FitnessEndCycle){
    cout<<endl<<"Fitness Population End Cycle:"<<endl;
    cout<<"\tMaximum Fitness: "<<InitialFitness+(PopulationTestTime)*WalkingPoints*0.5<<endl;
    cout<<"\tMinimum Fitness: "<<InitialFitness+(PopulationTestTime)*CollisionPoints<<endl;
    cout<<"\tSum Fitness: "<<totalFitness<<endl;
    cout<<"\tAverage Fitness: "<<totalFitness/NumRobots<<endl;
    cout<<"\tRobot "<<bestRobot<<" was the best robot with fitness equal to "<<bestRobotFitness<<endl<<endl;
    cout<<"\tRobots Fitness:"<<endl;
    for (int i = 1; i <= NumRobots; i++){
      cout<<"\t\tRobot "<<i<<": ";
      for (size_t j = 0; j < GenMeanFit; j++) {
          cout<<fitness[i][j]<<" ";
      }
      cout<<"avr("<<averageFitness[i]<<")"<<endl;
    }
  }

  for (int i = 1; i <= NumRobots; i++){
    if(fitness[i][GenMeanFit-1]<fitness[bestRobot][GenMeanFit-1]){
      if(ControlCrossing==1){
        for (int j = 0; j < 5; j++){
          chromosome[i][j] = (chromosome[i][j]+chromosome[bestRobot][j])/2;
        }
      }

      if(ControlBackMutationPrevention==1){
          for (int j = 0; j < 5; j++){
            BMP[i][j]=0;
          }
      }
      //----- Mutation -----//
      if(ControlMutation!=-1){
        for (int j = 0; j < 5; j++) {
          bool mutation = (rand()%100)<=ControlMutation;

          if(mutation && BMP[i][j]==0){
            BMP[i][j]=1;

            int checkBMP=0;
            for (int k = 0; k < 5; k++) {
              BMP[i][k]==1?checkBMP++:checkBMP;
            }
            if(checkBMP==5){
              for (int k = 0; k < 5; k++) {
                BMP[i][k]=0;
              }
            }

            double neut=0;
            if(ControlNeutralization!=-1){
              neut = double(ControlNeutralization)/100;
            }else{
              neut = 1;
            }

            showMutation[i][j]='*';
            switch(j){
              case 0:
                chromosome[i][0] = ( (1-neut)*chromosome[i][0] )+ ( (neut)*(double(rand() % 35)/10) );
              break;
              case 1:
                chromosome[i][1] = ( (1-neut)*chromosome[i][1]) + ( (neut)*(double(rand() % 100)/100) );
              break;
              case 2:
                chromosome[i][2] = ( (1-neut)*chromosome[i][2]) + ( (neut)*((rand() % 85)+5) );
              break;
              case 3:
                chromosome[i][3] = ( (1-neut)*chromosome[i][3]) + ( (neut)*(double(rand() % 50)/10) );
              break;
              case 4:
                chromosome[i][4] = ( (1-neut)*chromosome[i][4]) + ( (neut)*(double(rand() % 90)) );
              break;
              default:
              break;
            }
          }
        }
      }
    }
}
