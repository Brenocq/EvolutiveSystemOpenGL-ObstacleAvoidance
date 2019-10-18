// Window Parameters
#define windowHeight 600
#define windowWidth 600
#define screenHeight 1080
#define screenWidth 1920
#define displaySteps 3000

// Evolutive system global parameters (fixed)
#define qtdRobots 6
#define qtdEnvironments 10
#define qtdRepetitions 10// Number of times that each environment will be tested to define the fitness
#define qtdObstacles 40
#define pointsCollision -50.0// Points per collision
#define pointsRotation -2.0// Points every 360 degrees rotation
#define pointsMoving 10.0// Points per second in maximum speed
#define pointsExploration 1000// Points when explored all the world
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
// Set as -1 if you want to evolute the gene
#define controlFitnessMean 10
#define controlMutationRate .4
#define controlNeutralCrossing 0.6
#define controlNeutralMutation 0.3
#define controlBackMutationPrevention 1
#define controlCrossingCondition 0

// Robot info
#define controlQtdSensors 10
#define controlSensorQtdDivisions 10.0f// SensorValue: [0..n-1] when object | n when is not reading obstacle (n+1 divisions)
#define controlSensorMaxDist 1.5f// meters
#define controlEnableSensor 0.35f//  0 = always enable | 1 = never enable
