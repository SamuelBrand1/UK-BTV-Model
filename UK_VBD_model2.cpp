//Farm model for UK
//Making it simpler by considering every vector as a random walker
//Putting in optimization techniques --- vector object of past days exposed vectors with a reserved size

//Each farm has explicit demography


#include <vector>
#include <map>
#include <list>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <random>
#include <chrono>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>


#include "grids.h"
#include "Declarations.h"
#include "const_parameters.h"
#include "RandomFuncs.h"
#include "farm_defn.h"
#include "SetupFuncs.h"
#include "variable_parameters.h"
#include "SimulationFuncs.h"
#include "MidgeLifeAndDiffusion.h"
#include "TransportRisk.h"
#include "controls.h"

using namespace std;




int main(int argc,char** argv){
	
	
	//~ cout<<"Input = "<<argv[1]<<endl;
	//GSL Random variable set up
	
	 gsl_rng_env_setup();

	T = gsl_rng_default;
	r = gsl_rng_alloc (T);
	
	#ifdef NOFARMBAN //Placeholder values
	   BanRadius = -1;
    #endif
    
	 #ifdef NOCONTROL //Placeholder values
	   BanRadius = -1;
       PZRadius = 0;
       SZRadius = 0;
    #endif
	DiffusionWarning();
	DefineOutputFileName(argv);
	
	#ifdef PRINTOUTRESULTS
	ofstream OutputOfSimulations(OutputFilename);
	#endif
	
	ReadInAllData();
	
   
	
	for(int reps = 0; reps<NumOfReps ; reps++){
		ResetEpidemic(r,reps);
		cout<<"This is simulation number "<<reps + 1<<endl;
		
		
		while(SimulationDay < NumDays){
			NumberOfCattleInfectedToday = 0;
			NumberOfSheepInfectedToday = 0;
			NumOfFarmsDetectedToday = 0;
			SimulateEpidemicForOneDay(r);
			if(NumOfFarmsDetectedToday == 0){DaysSinceLastDetection++;}
			if(DaysSinceLastDetection == 90){CancelRestrictions();}
			TotalFarmDaysMovementBanned += TotalNumberOfMovementBannedFarms();
			TotalFarmDaysAffectedByControl += TotalNumberOfMovementBannedFarms() + TotalNumberOfPZFarms() + TotalNumberOfSZFarms();
			#ifdef DAILYREADOUT
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  			cout<<"Cattle incidence = "<<NumberOfCattleInfectedToday<<", Sheep incidence = "<<NumberOfSheepInfectedToday<<", detected farm incidence = "<<NumOfFarmsDetectedToday<<endl; 
			cout<<"S_C = "<<TotalNumberOfSusCattle()<<", I_C =  "<<TotalNumberOfInfCattle()<<", R_C = "<<TotalNumberOfRecCattle()<<endl; 
			cout<<"S_S = "<<TotalNumberOfSusSheep()<<", I_S =  "<<TotalNumberOfInfSheep()<<", R_S = "<<TotalNumberOfRecSheep()<<endl; 
			cout<<"Number of infected farms = "<<TotalNumberOfInfectedFarms()<<", number of affected farms = "<<TotalNumberOfAffectedFarms()<<endl;
			cout<<"Total number of movement ban farm days = "<<TotalFarmDaysMovementBanned<<", total number of interupted movements = "<<InterruptedMovements<<", total number of sheep deaths = "<<NumOfSheepDeaths<< endl;
			cout<<"Total number of transmitting movements = "<<NumberOfMovementTransmission<<", total number of interupted movements from risky farms = "<<NumberOfRiskyMovesBlocked<<endl;
			cout<<"BTV detected? "<<BTVObserved<<". Active surveillence performed? "<<ActiveSurveillancePerformed<<". Control zones set up ? "<<RestrictionZonesImplemented<<endl;
			cout<<"Total number of PZ farms = "<<TotalNumberOfPZFarms()<<", total number of SZ farms = "<<TotalNumberOfSZFarms()<<endl;
			cout<<"Number of farms actively checked = "<<NumberOfFarmsChecked<<", Number of ELISA tests sent in = "<<NumberOfTests<<", Number of pos. ELISA sent for PCR confirmation = "<<NumberOfPosTests<<endl;
			#endif
			
			if(SimulationDay == StartDayOfYear + 1 && reps == 0){PrintSpatialEpidemicOutToFile();PrintInfMidgeGridOutToFile();}
			if(SimulationDay == 190&& reps == 0){PrintSpatialEpidemicOutToFile();PrintInfMidgeGridOutToFile();}
			if(SimulationDay == 240&& reps == 0){PrintSpatialEpidemicOutToFile();PrintInfMidgeGridOutToFile();}
			if(SimulationDay == 270&& reps == 0){PrintSpatialEpidemicOutToFile();PrintInfMidgeGridOutToFile();}
			if(SimulationDay == 300&& reps == 0){PrintSpatialEpidemicOutToFile();PrintInfMidgeGridOutToFile();}
			//~ if(SimulationDay == 430&& reps == 0){PrintSpatialEpidemicOutToFile();PrintInfMidgeGridOutToFile();}
			//~ if(SimulationDay == 500&& reps == 0){PrintSpatialEpidemicOutToFile();PrintInfMidgeGridOutToFile();}
			//~ if(SimulationDay == 600&& reps == 0){PrintSpatialEpidemicOutToFile();PrintInfMidgeGridOutToFile();}
			//~ if(SimulationDay == 700&& reps == 0){PrintSpatialEpidemicOutToFile();PrintInfMidgeGridOutToFile();}


		}
	
	//Read out Sim results
		#ifdef PRINTOUTRESULTS
		OutputOfSimulations<<StartDayOfYear<<", "<<OutbreakCounty<<", "<<BanRadius<<", "<<PZRadius<<", "<<SZRadius;
		OutputOfSimulations<<", "<<TotalFarmDaysAffectedByControl<<", "<<TotalFarmDaysMovementBanned<<", "<<InterruptedMovements<<", "<<NumOfSheepDeaths<<", "<<TotalNumberOfInfCattle() + TotalNumberOfRecCattle()<<", "<<TotalNumberOfInfSheep() + TotalNumberOfRecSheep()<<", "<<TotalNumberOfAffectedFarms()<<", "<<NumberOfMovementTransmission;
		OutputOfSimulations<<", "<<NumberOfFarmsChecked<<", "<<NumberOfTests<<", "<<NumberOfPosTests;
		OutputOfSimulations<<", "<<DiffusionLengthScale<<", "<<PreferenceForSheep<<", "<<TransmissionScalar<<", "<<AttractPowScaler<<", "<<DiffMultScaler<<endl;
		#endif
	}
	  



	DestroyFarmPopulation();
	#ifdef PRINTOUTRESULTS
	OutputOfSimulations.close();
	#endif
}


