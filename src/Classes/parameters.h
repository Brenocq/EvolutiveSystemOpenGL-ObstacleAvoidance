// Window Parameters
#define windowHeight 600
#define windowWidth 600
#define screenHeight 1080
#define screenWidth 1920
#define displaySteps 10000 
#define worldSize 15

// Evolutive system global parameters (fixed)
#define qtdRobots 15
#define qtdEnvironments 10
#define qtdRepetitions 10// Number of times that each environment will be tested to define the fitness
#define qtdObstacles 120
#define pointsCollision -1000.0// Points per collision
#define pointsRotation 0// Points every 360 degrees rotation
#define pointsMoving 10.0// Points per second in maximum speed
#define pointsExploration 0// Points when explored all the world (not used)
#define maxPopulation 150// Number of populations tested with each environment
#define maxTime 1000// (MAX TIME TESTS) Time that each population will be tested
#define envMutationRate 0.2

// OutputFile
#define printEverything 1// 0->Print only essentials, 1->Print everything

// Debug
#define showAvFitness 0
#define showPopulationRobots 0
#define showPopulationEnvironments 1

// Fixed genes robots
// Set as -1 if you want to change
#define controlMaximumVelocity 0.8 
#define controlMaximumRotation 4 

// Fixed genes environments
// Set as -1 if you want to evolute the gene
#define controlFitnessMean 10// (HEREDITY)
#define controlMutationRate 0.2f
#define controlNeutralCrossing 0.3// Only when using elitism
#define controlQtdTournament 5// Only when using tournament
#define controlNeutralMutation 0.5
#define controlBackMutationPrevention 0
#define controlCrossingCondition 0

// Robot info
#define controlQtdSensors 3
#define controlMaxSensorAngle 120.0f
#define controlSensorQtdDivisions 10.0f// SensorValue: [0..n-1] when object | n when is not reading obstacle (n+1 divisions)
#define controlSensorMaxDist 2.0f// meters
#define controlEnableSensor 0.5f//  0 = always enable | 1 = never enable
