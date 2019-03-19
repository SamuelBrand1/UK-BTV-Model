#ifndef TRANSPORTRISK_H_INCLUDED
#define TRANSPORTRISK_H_INCLUDED

#include "const_parameters.h"

void TransmissionViaMovement(int FarmFrom, int FarmTo, double risk){		 
	// check if any movement occurs on day
	double rand;
	int SizeCattleMove,SizeSheepMove;
	double IncidenceDensitySheep,IncidenceDensityCattle;//These are the effective sizes for the cattle and or sheep movements
	double Selection,sum;
	int NumInfAnimalsMoved = 0;
	
	rand = RandU(RandGenerator);
	bool Transport=false;
	bool interrupt=false;
	bool CattleMove = false;
	bool SheepMove = false;

	//check if a movement is desired  on this day
	if(rand<=risk){Transport = true;}
	
	//check if there is movement interuption on an otherwise transport day
	if(Transport == true){
		//Criteria for interuption
		//Either farm is movement banned
		if(farms[FarmFrom]->MovementBanned == true || farms[FarmTo]->MovementBanned == true){interrupt = true;}
		//FarmFrom is in protection zone but FarmTo isn't
		if(farms[FarmFrom]->ProtectionZone == true && farms[FarmTo]->ProtectionZone == false){interrupt = true;}
		//FarmFrom is in Surveillance zone but FarmTo is in free area
		if(farms[FarmFrom]->SurveillanceZone == true && farms[FarmTo]->FreeArea == true){interrupt = true;}
	}

	//Movement occurs
	if(Transport == true && interrupt == false){
		//Choose whether it is a cattle or sheep move
		if(RandU(RandGenerator) > farms[FarmFrom]->NumOfSheep()/(farms[FarmFrom]->NumOfSheep() + farms[FarmFrom]->NumOfCattle())){CattleMove = true;}
		else{SheepMove = true;}
		
		//If it is a cattle movement then implement movement of the infectious cattle only
		if(CattleMove){
			SizeCattleMove = IntMin(1 + RandNegBin(r,0.125,1-0.0303),farms[FarmFrom]->NumOfCattle());
			for(int n = 0; n < SizeCattleMove; n++){
				IncidenceDensityCattle = (farms[FarmFrom]->NumOfInfCattle())/(farms[FarmFrom]->NumOfCattle());//Probability that an infectious cow is moved. Ignoring others
				if(RandU(RandGenerator) < IncidenceDensityCattle){//Picks an infected cattle to move
					Selection = RandU(RandGenerator)*farms[FarmFrom]->NumOfInfCattle();//Choose cow
					sum = 0;
					for(int k = 0; k < NumOfInfStagesCattle;k++)
					{
						sum += farms[FarmFrom]->I_cattle[k];
						if(sum >= Selection){ //Move a cow from that class
							farms[FarmFrom]->I_cattle[k] -= 1;
							farms[FarmTo]->I_cattle[k] += 1;
							NumInfAnimalsMoved++;
							break;
						}
					}
				}
			}
		}
		//If it is a sheep movement then implement movement of the infectious sheep only
		if(SheepMove){
			SizeSheepMove = IntMin(1 + RandNegBin(r,0.0883,1-0.003),farms[FarmFrom]->NumOfSheep());
			for(int n = 0; n < SizeSheepMove; n++){
				IncidenceDensitySheep = (farms[FarmFrom]->NumOfInfSheep())/(farms[FarmFrom]->NumOfSheep());
				if(RandU(RandGenerator) < IncidenceDensitySheep){//Picks an infected sheep to move
					Selection = RandU(RandGenerator)*farms[FarmFrom]->NumOfInfSheep();//Choose sheep
					sum = 0;
					for(int k = 0; k < NumOfInfStagesSheep;k++)
					{
						sum += farms[FarmFrom]->I_sheep[k];
						if(sum >= Selection){ //Move a sheep from that class
							farms[FarmFrom]->I_sheep[k] -= 1;
							farms[FarmTo]->I_sheep[k] += 1;
							NumInfAnimalsMoved++;
							break;
						}
					}
				}
			}
		}
	}

	//Movement is banned
	if(Transport == true && interrupt == true){
		InterruptedMovements++;	
		if(farms[FarmFrom]->NumOfInfCattle() + farms[FarmFrom]->NumOfInfSheep() > 0){NumberOfRiskyMovesBlocked++;}
	}
	#ifdef VERBOSE
	if(NumInfAnimalsMoved >0){
	std::cout<<"In TransmissionViaMovement"<<" FarmFrom = "<<FarmFrom<<" FarmTo = "<<FarmTo<<", Number inf moved = "<<NumInfAnimalsMoved<< std::endl;
	}
	#endif
	if(NumInfAnimalsMoved > 0 ){NumberOfMovementTransmission++;}
}

void MovementTransmission(){
	int FarmTo;
	double risk;
	for(int k = 0;k<N;k++){
		//~ if(farms[k]->NumOfInfCattle() + farms[k]->NumOfInfSheep() > 0){
			for(int n = 0; n < farms[k]->movement_local_farms.size() ; n++){
				FarmTo = int(farms[k]->movement_local_farms[n][0]);//Gets ID of farm being transmitted to
				risk = farms[k]->movement_local_farms[n][1]; //Risk that movement based transmission occurs
				//Generate movement event
				TransmissionViaMovement( k , FarmTo, risk);
			}
		//~ }
	}
}


#endif

