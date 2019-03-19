#ifndef DECLARATIONS_H_INCLUDED
#define DECLARATIONS_H_INCLUDED

//Useful declarations

std::string read_in_string;
int iter;
std::stringstream ss;

double a1,a2,a3,a4,a5,a6,a7;
int b1,b2,b3;
  
double L[3][3]; //Lower Cholesky decomposition giving correct covariance structure for Location level random effect

//Variables that keep track of the epidemic
int SimulationDay;
int InterruptedMovements, DaysOfMovementBan,NumOfSheepDeaths ,NumberOfFarmsChecked, NumberOfTests, NumberOfPosTests,TotalFarmDaysMovementBanned,TotalFarmDaysAffectedByControl ;
int NumberOfSheepInfectedToday,NumberOfCattleInfectedToday;
int FirstDetectedFarmID;
bool BTVObserved,ActiveSurveillancePerformed,RestrictionZonesImplemented;
int DaysSinceLastDetection,NumOfFarmsDetectedToday;
char OutputFilename[100];
int NumberOfMovementTransmission;
int NumberOfRiskyMovesBlocked;

#endif

