#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include "utils.h"
using namespace std;

class Environment{
public:
  Environment(int _qtdRobots, int _qtdObstacles, int _maxTime, int _maxPopulation, int _pointsMoving, int _pointsCollision);
  void updateRobots(float time);
  void initiateRobots();
  void newPopulationRobots();
  void newRandomPositions();

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
private:
  //----- Simulation parameters -----//
  int qtdRobots;
  int qtdObstacles;
  int currTime;// current time in the simulation
  int maxTime;// Time that each population will be tested
  int currentPopulation;// Number of the current population
  int maxPopulation;// Maximum number of populations tested
  int pointsMoving;// Points for moving
  int pointsCollision;// Points when colliding
};

#endif // ENVIRONMENT_H
