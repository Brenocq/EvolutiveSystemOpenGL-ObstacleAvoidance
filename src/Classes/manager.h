#ifndef MANAGER_H
#define MANAGER_H
#include <iostream>
#include "environment.h"
#include "parameters.h"
using namespace std;

class Manager{
public:
  Manager();
  void initiateEnvironments();
  void updateEnvironment(float time);
  void newPopulationEnvironments();
  void draw();
  //----- Getters and Setters -----//
  int getCurrEnvironment()const {return currEnvironment;}
  int getCurrRepetition()const {return currRepetition;}

  //----- Manager entities -----//
  vector<Environment> environment;
private:
  //----- Simulation parameters -----//
  int currEnvironment;
  int currRepetition;
};

#endif // MANAGER_H
