#include "manager.h"

Manager::Manager():
  currEnvironment(0), currRepetition(0)
{
  envMeanFitness.resize(1);
  for (int i = 0; i < qtdEnvironments; i++) {
    Environment env;
    environment.push_back(env);
  }
  initiateEnvironments();
}

void Manager::initiateEnvironments(){
  for (int i = 0; i < qtdEnvironments; i++){
    // fitnessMean   (0.0-10.0)qtdRobots
    if(controlFitnessMean!=-1)
      environment[i].genes[0] = controlFitnessMean;
    else
      environment[i].genes[0] = (rand()%1000)/100.0;

    // mutationRate  (0.0-1.0)
    if(controlMutationRate!=-1)
      environment[i].genes[1] = controlMutationRate;
    else
      environment[i].genes[1] = (rand()%100)/100.0;

    // neutralCrossing (0.0-1.0)
    if(controlNeutralCrossing!=-1)
      environment[i].genes[2] = controlNeutralCrossing;
    else
      environment[i].genes[2] = (rand()%100)/100.0;

    // neutralMutation (0.0-1.0)
    if(controlNeutralMutation!=-1)
      environment[i].genes[3] = controlNeutralMutation;
    else
      environment[i].genes[3] = (rand()%100)/100.0;

    // controlBackMutationPrevention (0.0-1.0) (<0.5)(>=0.5)
    if(controlBackMutationPrevention!=-1)
      environment[i].genes[4] = controlBackMutationPrevention;
    else
      environment[i].genes[4] = (rand()%100)/100.0;

    // crossingCondition  (0.0-3.0) (<1)(<2)(<3)
    if(controlCrossingCondition!=-1)
      environment[i].genes[5] = controlCrossingCondition;
    else
      environment[i].genes[5] = (rand()%300)/100.0;

    environment[i].fitness.push_back(0);
  }
}

void Manager::updateEnvironment(float time){
  if(environment[currEnvironment].getFinished()){
    // Next repetition
    currRepetition++;
    if(currRepetition==qtdRepetitions){
      cout<<"Mean: "<<envMeanFitness.back()<<endl;
      envMeanFitness.push_back(0);

      currRepetition = 0;
      // Next environemnt
      currEnvironment++;
      if(currEnvironment==qtdEnvironments){
        // Reset environemnts
        newPopulationEnvironments();
        currEnvironment=0;
      }
    }
  }

  environment[currEnvironment].updateRobots(time);

  if(environment[currEnvironment].getFinished()){
    envMeanFitness.back()=((currRepetition)*envMeanFitness.back()+environment[currEnvironment].fitness.back())/(currRepetition+1);

    cout<<"Enviroment ("<<currEnvironment<<"/"<<qtdEnvironments-1<<")"<<
    " - Repetition("<<currRepetition<<"/"<<qtdRepetitions-1<<"): "
    << environment[currEnvironment].fitness.back()<<endl;
    for (size_t i = 0; i < qtdRobots; i++) {
      environment[currEnvironment].robot[i].meanFitness.clear();
      environment[currEnvironment].robot[i].fitness.clear();
    }
  }
}

void Manager::newPopulationEnvironments(){
  vector< pair<int,int> >bestEnvironments;
  int bestEnvironment;

  for (int i = 0; i < qtdEnvironments; i++){
    bestEnvironments.push_back(make_pair(environment[i].fitness.back(),i));
  }
  sort(bestEnvironments.rbegin(),bestEnvironments.rend());

  bestEnvironment = bestEnvironments[0].second;
  for (int i = 0; i < qtdEnvironments; i++) {
    for (int j = 0; j < int(environment[0].genes.size()); j++) {
      environment[i].genes[j] = (environment[i].genes[j] + environment[bestEnvironment].genes[j])/2;
    }
  }

  //----- Mutation -----//
  for (int i = 0; i < qtdEnvironments; i++) {
    if(environment[i].fitness.back()<environment[bestEnvironment].fitness.back()){
      for (int j = 0; j < int(environment[0].genes.size()); j++) {
      int chanceMutation = rand()%100;
        if(chanceMutation < envMutationRate*100){
          switch(j){
            case 0:
            if(controlFitnessMean==-1)
              environment[i].genes[0] = (rand()%1000)/100.0;
            break;
            case 1:
            if(controlMutationRate==-1)
              environment[i].genes[1] = (rand()%100)/100.0;
            break;
            case 2:
            if(controlNeutralCrossing==-1)
              environment[i].genes[2] = (rand()%100)/100.0;
            break;
            case 3:
            if(controlNeutralMutation==-1)
              environment[i].genes[3] = (rand()%100)/100.0;
            break;
            case 4:
            if(controlBackMutationPrevention==-1)
              environment[i].genes[4] =  (rand()%100)/100.0;
            break;
            case 5:
            if(controlCrossingCondition==-1)
              environment[i].genes[5] = (rand()%300)/100.0;
            break;
          }
        }
      }
    }
  }
}

void Manager::draw(){
  environment[currEnvironment].draw();
}
