// Window Parameters
#define windowHeight 600
#define windowWidth 600
#define screenHeight 1080
#define screenWidth 1920
#define displaySteps 1000

// Evolutive system global parameters (fixed)
#define qtdRobots 15
#define qtdEnvironments 10
#define qtdRepetitions 10// Number of times that each environment will be tested to define the fitness
#define qtdObstacles 50
#define pointsCollision -10.0// Points per second in collision
#define pointsMoving 10.0// Points per second in maximum speed
#define maxPopulation 500// Number of populations tested with each environment
#define maxTime 600// Time that each population will be tested
#define envMutationRate 0.2

// OutputFile
#define printEverything 1// 0->Print only essentials, 1->Print everything

// Debug
#define showAvFitness 0
#define showPopulationRobots 0
#define showPopulationEnvironments 1

// Fixed genes robots
// Set as -1 if you want to change
#define controlMaximumVelocity -1
#define controlMaximumRotation -1

// Fixed genes environments
// Set as -1 if you want to change
#define controlFitnessMean 3
#define controlMutationRate .2
#define controlNeutralCrossing 0.5
#define controlNeutralMutation 0.5
#define controlBackMutationPrevention 1
#define controlCrossingCondition 1

// Robot info
#define controlQtdSensors 5
#define controlSensorQtdDivisions 4// SensorValue: [0..n-1] when object | n when is not reading obstacle (n+1 divisions)
#define controlSensorMaxDist 3// meters
#define controlEnableSensor 0.3//  0 = always enable | 1 = never enable
