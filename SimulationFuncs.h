
#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED
#include "RandomFuncs.h"
#include "MidgeLifeAndDiffusion.h"
#include "SetupFuncs.h"
#include "TransportRisk.h"
#include "const_parameters.h"
#include "Declarations.h"
#include "controls.h"

//Functions that do simulation and reading out of simulation results

//~ double NumOfInfCattle(){
	//~ sum = 0;
	//~ for(int k = 0;k<N;k++){
		
	//~ }
//~ }


void SpatialAutoCorrOnDay(){
	double ACGrid[NumGridPointsACS][NumGridPointsACE];
	for(int i =0 ; i<NumGridPointsACS ; i++){
		for(int j =0 ; j<NumGridPointsACE ; j++){
			ACGrid[i][j] = 0.7540*RandN(RandGenerator);
		}
	}
}

void ReadInAllData(){
	ReadInTempData();
	ReadInRainData();
	ReadInCovarianceMatrix();
	ReadInLHSamples();
	ConstructFarmPopulation();
	ReadInFarmData();
	ReadInMovementData();
}

void ResetEpidemic(const gsl_rng * r,int RN){
	std::cout<<"Reseting epidemic"<<std::endl;
	ResetFarms();
	ResetMidgeGrid();
	#ifdef GENERATEPARAMETERSRANDOMLY
	DrawParamsFromPrior(r);
	#endif
	#ifdef USEDEFINEDPARAMETERS
	DrawParamsFromLHSamples(RN);
	#endif
	
	SolveFarmBitingPreference(PreferenceForSheep,AttractPowScaler);
	SimulationDay = StartDayOfYear;
	InterruptedMovements = 0;
	DaysOfMovementBan = 0;
	TotalFarmDaysMovementBanned = 0;
	TotalFarmDaysAffectedByControl = 0;
	NumOfSheepDeaths = 0;
	NumberOfFarmsChecked= 0;
	NumberOfTests = 0;
	NumberOfPosTests = 0;
	NumOfSheepDeaths = 0;
	NumberOfMovementTransmission = 0;
	NumberOfRiskyMovesBlocked = 0;
	BTVObserved = false;
	ActiveSurveillancePerformed = false;
	RestrictionZonesImplemented = false;
	InitialiseEpidemicAtCounty();
}



void SimulateEpidemicForOneDay(const gsl_rng * r){
	#ifdef DAILYREADOUT
	std::cout<<"Simulating day "<<SimulationDay<<std::endl;
	#endif
	//Check if global control measure are to be implemented
	if(BTVObserved && !RestrictionZonesImplemented){
		SetUpRestrictionZone(FirstDetectedFarmID, PZRadius, SZRadius);
		if(!ActiveSurveillancePerformed)
		{
			PerformActiveSurveillance();
		}
	}
	MidgeMortalityAndIncubation();
	MovementTransmission();
	MidgeDiffusionForADay();
	SpatialAutoCorrOnDay();
	for(int k = 0;k<N;k++){
		farms[k]->GetWeatherToday();
		farms[k]->AnimalDeathsAndRecoveries(r);
		farms[k]->TransmissionMidgesToHosts(r);
		farms[k]->TransmissionHostsToMidges();
	}
	SimulationDay++;
}

void DefineOutputFileName(char** file_end){
	ss.clear();
    ss.str("");
    bool NormalOutput = true;
    
    #ifdef USEDEFINEDPARAMETERS

	        ss<<"./DATA/LHS_Results_L_"<<DiffusionLengthScale<<"_BR_"<<BanRadius<<"_init_CN_"<<OutbreakCounty<<"_end_day_"<<NumDays<<"_PZ_"<<PZRadius<<"_SZ_"<< SZRadius<<"_"<<file_end[1]<<".txt";
	        ss>> OutputFilename;
	
	#endif
	
	#ifdef GENERATEPARAMETERSRANDOMLY
	
			
	        ss<<"./DATA/RAND_Results_L_"<<DiffusionLengthScale<<"_BR_"<<BanRadius<<"_init_CN_"<<OutbreakCounty<<"_end_day_"<<NumDays<<"_PZ_"<<PZRadius<<"_SZ_"<< SZRadius<<"_"<<file_end[1]<<".txt";
	        ss>> OutputFilename;
		
	#endif
}


void PrintSpatialEpidemicOutToFile(){
	char buffer[100];
	std::stringstream filenamess; 
	filenamess.clear();
	filenamess.str("");
	filenamess<<"./IMAGE/AffectedFarmsImageDay_"<<SimulationDay<<".txt";
	filenamess>>buffer;
	std::ofstream out(buffer);
	for(int i = 0; i<N;i++){
		if(farms[i]->NumOfInfCattle() + farms[i]->NumOfInfSheep() + farms[i]->R_cattle + farms[i]->R_sheep > 0){
			out<<farms[i]->x[0]<<", "<<farms[i]->x[1]<<", "<<farms[i]->S_cattle<<", "<<farms[i]->NumOfInfCattle()<<", "<<farms[i]->R_cattle<<", "<<farms[i]->S_sheep<<", "<<farms[i]->NumOfInfSheep()<<", "<<farms[i]->R_sheep<<std::endl;
		}
	}
	out.close();
}

//Functions for reading out the state of the system

double TotalNumberOfSusCattle(){
	double sum = 0;
	for(int k = 0;k<N;k++){
		sum += farms[k]->S_cattle;
	}
	return sum;
}

double TotalNumberOfInfCattle(){
	double sum = 0;
	for(int k = 0;k<N;k++){
		sum += farms[k]->NumOfInfCattle();
	}
	return sum;
}

double TotalNumberOfRecCattle(){
	double sum = 0;
	for(int k = 0;k<N;k++){
		sum += farms[k]->R_cattle;
	}
	return sum;
}

double TotalNumberOfSusSheep(){
	double sum = 0;
	for(int k = 0;k<N;k++){
		sum += farms[k]->S_sheep;
	}
	return sum;
}

double TotalNumberOfInfSheep(){
	double sum = 0;
	for(int k = 0;k<N;k++){
		sum += farms[k]->NumOfInfSheep();
	}
	return sum;
}

double TotalNumberOfRecSheep(){
	double sum = 0;
	for(int k = 0;k<N;k++){
		sum += farms[k]->R_sheep;
	}
	return sum;
}

double TotalNumberOfMovementBannedFarms(){
	double sum = 0;
	for(int k = 0;k<N;k++){
		if(farms[k]->MovementBanned){sum = sum+1;}
	}
	return sum;
}

double TotalNumberOfPZFarms(){
	double sum = 0;
	for(int k = 0;k<N;k++){
		if(farms[k]->ProtectionZone){sum = sum+1;}
	}
	return sum;
}

double TotalNumberOfSZFarms(){
	double sum = 0;
	for(int k = 0;k<N;k++){
		if(farms[k]->SurveillanceZone){sum = sum+1;}
	}
	return sum;
}

double TotalNumberOfInfectedFarms(){
	double sum = 0;
	for(int k = 0;k<N;k++){
		if(farms[k]->NumOfInfCattle() + farms[k]->NumOfInfSheep() > 0){sum = sum+1;}
	}
	return sum;
}

double TotalNumberOfAffectedFarms(){
	double sum = 0;
	for(int k = 0;k<N;k++){
		if(farms[k]->NumOfInfCattle() + farms[k]->NumOfInfSheep() + farms[k]->R_cattle + farms[k]->R_sheep > 0){sum = sum+1;}
	}
	return sum;
}


#endif
