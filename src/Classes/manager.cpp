#include "manager.h"

Manager::Manager():
  currEnvironment(0), currRepetition(0)
{
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
    cout<<"Enviroment ("<<currEnvironment<<"/"<<qtdEnvironments-1<<")"<<
    " - Repetition("<<currRepetition<<"/"<<qtdRepetitions-1<<"): "
    << environment[currEnvironment].fitness.back()<<endl;
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
    cout<<"Environment ("<<environmentBeingTested<<"/"<<qtdEnvironments-1<<") - Repetition ("<<
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
  if(environmentBeingTested<qtdEnvironments){
    setEnvironmentParameters();
  }
}
//----- Tested all environments -----//
if(environmentBeingTested==qtdEnvironments){
  if(showPopulationEnvironments){
    cout<<"FINISH TESTING ENVIRONMENTS (Generation "<< populationEnvironmentNum <<")"<<endl;
    for (int i = 0; i < qtdEnvironments; i++) {
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
