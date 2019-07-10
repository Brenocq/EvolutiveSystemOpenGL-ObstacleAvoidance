#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
using namespace std;

class Environment{
public:
  Environment();
  vector<float> fitness;
  vector<float> auxFitness;// The mean will be added to the real fitness
  vector<float> genes;
  // fitnessMean = 5 // (int) Number of fitness values that will be used to calculate the meanFitness
  // mutationRate = 0.1 // Chance of each gene suffer mutation
  // neutralCrossing = 0.5 // 0 -> became the best robot, 1-> do not evolve
  // neutralMutation = 0.5 // 0 -> change to mutation, 1-> do not mutate
  // controlBackMutationPrevention = 1 // (bool) Control if will use back mutation prevention
  // crossingCondition = 0 // (int) 0:fitness<fitness, 1:fitness<meanFitness, 2:meanFitness<meanFitness
};

#endif // ENVIRONMENT_H
