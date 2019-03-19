#ifndef CONTROLS_H_INCLUDED
#define CONTROLS_H_INCLUDED

#include "const_parameters.h"
#include "farm_defn.h"

std::vector<int> ActiveSurveillanceFarms;
//Movement ban controls

void ImplementLocalMovementBan(int CentreFarm){
	int BannedFarmID;
	#ifdef VERBOSE
	std::cout<<"Implementing movement ban around farm "<<CentreFarm<<std::endl;
	#endif
		//Here I implement ring movement bans
		//If farm has never been detected before than construct its local farm list
	if(!farms[CentreFarm]->EverBeenDetectedDuringRun){FindLocalFarms(CentreFarm);farms[CentreFarm]->EverBeenDetectedDuringRun = true;}
	for(int n =0;n<farms[CentreFarm]->LocalFarmsID.size();n++){
		BannedFarmID = farms[CentreFarm]->LocalFarmsID[n];
		#ifndef NOFARMBAN
		farms[BannedFarmID]->MovementBanned = true;
		farms[BannedFarmID]->FreeArea = false;
		#endif
	}
	#ifdef RESTRICTIONZONES
	for(int k = 0; k<N; k++){
		if( DistSqBetweenFarms(k,CentreFarm) <= PZRadius*PZRadius){
			farms[k]->ProtectionZone = true;
			farms[k]->FreeArea = false;
		}
		if( DistSqBetweenFarms(k,CentreFarm) > PZRadius*PZRadius  &&  DistSqBetweenFarms(k,CentreFarm) <= SZRadius*SZRadius ){
			farms[k]->SurveillanceZone = true;
		    farms[k]->FreeArea = false;
		}	
	}
	#endif
	


//If there is a county ban then ban movement of all farms in same county
	#ifdef COUNTYBAN
	double CN = farms[CentreFarm]->CountyNumber;
	for(int k = 0; k<N ; k++){
	if(farms[k]->CountyNumber == CN){
		farms[k]->MovementBanned = true;
		farms[k]->FreeArea = false;
		} 
	}
	#endif
	//If there is a total ban policy
	#ifdef TOTALBAN
	for(int k = 0; k<N ; k++){
		farms[k]->MovementBanned = true;
		farms[k]->FreeArea = false;
		}
	#endif
}


//Set up standard extra controls :  Protection zone and Surveillance zone

void SetUpRestrictionZone(int FirstDetectedFarmID, double R, double R2){
	double SurvRadius = 15000;
	for(int k = 0; k<N; k++){
		if( DistSqBetweenFarms(k,FirstDetectedFarmID) <= R*R){
			farms[k]->ProtectionZone = true;
			farms[k]->FreeArea = false;
		}
		if( DistSqBetweenFarms(k,FirstDetectedFarmID) > R*R  &&  DistSqBetweenFarms(k,FirstDetectedFarmID) <= R2*R2 ){
			farms[k]->SurveillanceZone = true;
		    farms[k]->FreeArea = false;
		}	
			//Add farms for local active surveillance
		if( DistSqBetweenFarms(k,FirstDetectedFarmID) <= SurvRadius*SurvRadius){
			ActiveSurveillanceFarms.push_back(k);
		}
	}
	RestrictionZonesImplemented = true;
}



//Function for perform active surveillance
void PerformActiveSurveillance(){
	int FarmID;
	NumberOfFarmsChecked += ActiveSurveillanceFarms.size();
	for(int k = 0; k< ActiveSurveillanceFarms.size(); k++){
		FarmID = ActiveSurveillanceFarms[k];
		NumberOfTests += int(farms[FarmID]->NumOfCattle() + farms[FarmID]->NumOfSheep());
		if((farms[FarmID]->NumOfInfCattle() + farms[FarmID]->NumOfInfSheep()) > 0){
			farms[FarmID]->Detected = true;
			NumberOfPosTests += int(farms[FarmID]->NumOfInfCattle() + farms[FarmID]->NumOfInfSheep() + farms[FarmID]->R_sheep + farms[FarmID]->R_cattle);
		}
	}
	ActiveSurveillanceFarms.clear();
	ActiveSurveillancePerformed = true;
}


		
//Cancel restrictions on movements
void CancelRestrictions(){
	BTVObserved = false;
	ActiveSurveillancePerformed = false;
	RestrictionZonesImplemented = false;
	for(int k = 0; k<N; k++){
		farms[k]->MovementBanned = false;
		farms[k]->ProtectionZone = false;
		farms[k]->SurveillanceZone = false;
		farms[k]->Detected = false; 
		farms[k]->MovementBanned = false; 
		farms[k]->FreeArea = true;
	}
}

#endif
