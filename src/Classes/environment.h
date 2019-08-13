#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <GL/glut.h>
#include <vector>
#include <stdlib.h>// srand, rand
#include <iostream>// srand, rand
#include <time.h>
#include <algorithm>// sort
#include "parameters.h"
#include "utils.h"
#include "object.h"
#include "robot.h"
#include "obstacle.h"
using namespace std;

class Environment{
public:
  Environment();
  void updateRobots(float time);
  void initiateRobots();
  void newPopulationRobots();
  void newRandomPositions();
  void draw();
  //----- Getters and Setters -----//
  bool getFinished() const;
  int getCurrentPopulation()const{return currentPopulation;}
  float getCurrTime()const{return currTime;}
  //----- Environment entities -----//
  vector<Robot> robot;
  vector<Obstacle> obstacle;
  //----- Genes -----//
  vector<float> genes;// Environment gene
  // 0-fitnessMean                     (ex:5)   // (int) Number of fitness values that will be used to calculate the meanFitness
  // 1-mutationRate                    (ex:0.1) // (float) Chance of each gene suffer mutation
  // 2-neutralCrossing                 (ex:0.5) // (float) 0 -> became the best robot, 1-> do not evolve
  // 3-neutralMutation                 (ex:0.5) // (float) 0 -> change to mutation, 1-> do not mutate
  // 4-controlBackMutationPrevention   (ex:1)   // (bool) Control if will use back mutation prevention
  // 5-crossingCondition               (ex:0)   // (int) 0:fitness<fitness, 1:fitness<meanFitness, 2:meanFitness<meanFitness
  //----- Fitness -----//
  vector<int> fitness;// Environment fitness
private:
  bool finished;
  //----- Simulation parameters -----//
  float currTime;// current time in the simulation
  int currentPopulation;// Number of the current population
};

#endif // ENVIRONMENT_H
