#ifndef SETUPFUNCS_H_INCLUDED
#define SETUPFUNCS_H_INCLUDED

#include "const_parameters.h"
#include "farm_defn.h"
#include "RandomFuncs.h"
#include "variable_parameters.h"
#include "grids.h"
//Functions for initialising epidemics and reading in data

 //~ //Read in temperature data
 
void ReadInTempData(){ 
	std::cout<<"Loading temperature data"<<std::endl;
	std::ifstream Temp_in("UKTempTextFile0708.txt");
	iter = 0;
	while (getline(Temp_in,read_in_string))
	{
		ss.clear();
		ss.str("");
		ss<<read_in_string;
		ss>>a1;
		//Get the multidimensional grid co-ords
		b1 = iter%NumGridPointsS;
		b2 = int(((iter-b1)%(NumGridPointsS*NumGridPointsE))/NumGridPointsS);
		b3 = int( (iter - b1 - b2*NumGridPointsS)/(NumGridPointsE*NumGridPointsS)  );
		//~ std::cout<<b1<<" "<<b2<<" "<<b3<<std::endl;
		if(b3 < NumDays){
			TempGrid[b1][b2][b3] = a1;
		}			
		iter++;
	}
	Temp_in.close();
}

void ReadInRainData(){
	 //Read in mean rainfall over last week data
 	std::cout<<"Loading Rain data"<<std::endl;
	std::ifstream Rain_in("MeanRainFallLastWeekTextFile0708.txt");
	iter = 0;
	while (getline(Rain_in,read_in_string))
	{
		ss.clear();
		ss.str("");
		ss<<read_in_string;
		ss>>a1;
		//Get the multidimensional grid co-ords
		b1 = iter%NumGridPointsRainS;
		b2 = int(((iter-b1)%(NumGridPointsRainS*NumGridPointsRainE))/NumGridPointsRainS);
		b3 = int( (iter - b1 - b2*NumGridPointsRainS)/(NumGridPointsRainE*NumGridPointsRainS)  );
		if(b3 < NumDays){
			RainGrid[b1][b2][b3] = a1;		
		}	
		iter++;
	}
	
	Rain_in.close();
}

void ReadInCovarianceMatrix(){
	//Read in the location level correlation structure 
	std::cout<<"Loading random effects matrix"<<std::endl;
	std::ifstream L_in("LowerCholForLocationLevelRandomEffects.txt");
	iter = 0;
	while (getline(L_in,read_in_string))
	{
		ss.clear();
		ss.str("");
		ss<<read_in_string;
		ss>>a1>>a2>>a3;
		//Get the multidimensional grid co-ords
		L[iter][0] = a1;
		L[iter][1] = a2;
		L[iter][2] = a3;
		iter++;
	}
	L_in.close();
}

void ReadInLHSamples(){
	//Read in the location level correlation structure 
	std::cout<<"Loading latin hypercube samples"<<std::endl;
	std::ifstream L_in("LHSForBTVSims.txt");
	iter = 0;
	while (getline(L_in,read_in_string))
	{
		ss.clear();
		ss.str("");
		ss<<read_in_string;
		ss>>a1>>a2>>a3>>a4;
		//Get the multidimensional grid co-ords
		LHSParameters[iter][0] = a1;
		LHSParameters[iter][1] = a2;
		LHSParameters[iter][2] = a3;
		LHSParameters[iter][3] = a4;
		iter++;
	}
	L_in.close();
}

void ReadInFarmData(){
//~ //Read in demographic data
	std::cout<<"Loading farm attributes"<<std::endl;
	std::ifstream demog_in("Farms_2010_rounded.txt");
	iter = 0;
	while (getline(demog_in,read_in_string))
	{
		ss.clear();
		ss.str("");
		ss<<read_in_string;	
		ss>>a1>>a2>>a3>>a4>>a5>>a6>>a7;		
		farms[iter]->CountyNumber = a1;
		farms[iter]->x[0] = a4;
		farms[iter]->x[1] = a5;
		farms[iter]->number_of_sheep = a6;
		farms[iter]->number_of_cattle = a7;	
		//Get grid location
		farms[iter]->TempGrid_x = int(floor(farms[iter]->x[0]/TempGridWidth));	
		farms[iter]->TempGrid_y = int(floor(farms[iter]->x[1]/TempGridWidth));
		farms[iter]->RainGrid_x = int(floor(farms[iter]->x[0]/RainGridWidth));	
		farms[iter]->RainGrid_y = int(floor(farms[iter]->x[1]/RainGridWidth));	
		farms[iter]->MidgeGrid_x = int(floor(farms[iter]->x[0]/MidgeGridWidth));	
		farms[iter]->MidgeGrid_y = int(floor(farms[iter]->x[1]/MidgeGridWidth));	
		farms[iter]->ACGrid_x = int(floor(farms[iter]->x[0]/AutoCorrGridWidth));	
		farms[iter]->ACGrid_y = int(floor(farms[iter]->x[1]/AutoCorrGridWidth));		
		
		//~ if(farms[iter]->MidgeGrid_y > (NumGridPointsS-1)){std::cout<<"TOO BIG y temp grid"<<std::endl;}
		//~ if(farms[iter]->MidgeGrid_x > (NumGridPointsE-1)){std::cout<<"TOO BIG x temp grid"<<std::endl;}
		
		//~ if(farms[iter]->RainGrid_y > (NumGridPointsRainS-1)){std::cout<<"TOO BIG y Rain grid"<<std::endl;}
		//~ if(farms[iter]->RainGrid_x > (NumGridPointsRainE-1)){std::cout<<"TOO BIG x Rain grid"<<std::endl;}

		iter++;	
	}	
	demog_in.close();
}

void ReadInMovementData(){
	std::cout<<"Loading movement data for farms"<<std::endl;
	  //Initialise the move element
	std::vector<double> MoveToAndRisk;
	MoveToAndRisk.push_back(-99);
	MoveToAndRisk.push_back(-99);
	//~ std::cout<<"Loading movement Data"<<std::endl;
	std::ifstream move_in("movements_unique.txt");
	
	while (getline(move_in,read_in_string))
	{
		ss.clear();
		ss.str("");
		ss<<read_in_string;
		
		ss>>a1>>a2>>a3;
		MoveToAndRisk[0] = a2-1;
		MoveToAndRisk[1] = a3;
		
		farms[int(a1)-1]->movement_local_farms.push_back(MoveToAndRisk);  
		 //~ std::cout<<"  "<<MoveToAndRisk[0]<<"  "<<MoveToAndRisk[1]<<std::endl;
	
	}
	
	move_in.close();
	
	//~ std::cout<<farms[1]->movement_local_farms[0][0]<<"  "<<farms[1]->movement_local_farms[0][1]<<std::endl;
}


//Constructs the farm population and gives them their fixed random effects
void ConstructFarmPopulation(){
	std::cout<<"Constructing population of farms"<<std::endl;	
	for(int i = 0;i<N;i++){
		farms[i] = new farm;
		farms[i]->ID = i;
		farms[i]->EverBeenDetectedDuringRun = false;
		//~ farms[i]->V_intercept = (4.0519 - 11.5931);
		//~ farms[i]->temp_eff = 1.2687;
		//~ farms[i]->wind_eff = -0.26922;
		//~ farms[i]->sin_yearly = -0.9064;
		//~ farms[i]->cos_yearly = 4.1756;
		//~ farms[i]->sin_6_month = -0.6901;
		//~ farms[i]->cos_6_month = 2.105;
		//~ farms[i]->cos_4_month = 1.9411;
		//~ farms[i]->temp_eff_sq = -0.02919;
		//~ farms[i]->rain_eff = 0.2391;
		farms[i]->V_intercept = -2.56;
		farms[i]->temp_eff = 0.07;
		farms[i]->wind_eff = -0.18;
		farms[i]->sin_yearly = -1.64;
		farms[i]->cos_yearly = -2.92;
		farms[i]->sin_6_month = -1.19;
		farms[i]->cos_6_month = -0.7;
		farms[i]->cos_4_month = 0.0;
		farms[i]->temp_eff_sq = 0.0;
		farms[i]->rain_eff = 0.0;
	}
}

void DestroyFarmPopulation(){
	  for (int k=0; k<N; k++) {
        delete  farms[k];
    }
}



void ResetMidgeGrid()
{
	for(int i = 0; i<NumGridPointsMidgeS;i++){
		for(int j = 0; j<NumGridPointsMidgeE;j++){
			InfMidgeDensity[i][j] = 0;
			for(int k =0;k<NumOfEIPStages;k++){
				LatentMidgeDensity[i][j][k] = 0;
			}
			DiffusionGrid[i][j] = 0;
		}
	}
}

void ResetFarms(){
	double w1,w2,w3;
	for(int i = 0;i<N;i++){
		farms[i]->DayOfYear = StartDayOfYear;
		//Generate the correlated farm level random effects
		w1 = RandN(RandGenerator);w2 = RandN(RandGenerator);w3 = RandN(RandGenerator);
		//~ farms[i]->V_intercept += L[0][0]*w1;
		//~ farms[i]->temp_eff += L[1][0]*w1 + L[1][1]*w2;
		//~ farms[i]->wind_eff += L[2][0]*w1 + L[2][1]*w2 + L[2][2]*w3;
		
		//Reset to baseline parameters
		
		farms[i]->V_intercept = -2.56;
		farms[i]->temp_eff = 0.07;
		farms[i]->wind_eff = -0.18;
		farms[i]->sin_yearly = -1.64;
		farms[i]->cos_yearly = -2.92;
		farms[i]->sin_6_month = -1.19;
		farms[i]->cos_6_month = -0.7;
		farms[i]->cos_4_month = 0.0;
		farms[i]->temp_eff_sq = 0.0;
		farms[i]->rain_eff = 0.0;
		
		//Add random effect
		
		farms[i]->V_intercept += 0.79*RandN(RandGenerator);
		farms[i]->temp_eff += 0.05*RandN(RandGenerator);
		farms[i]->wind_eff += 0.07*RandN(RandGenerator);
		farms[i]->sin_yearly += 0.23*RandN(RandGenerator);
		farms[i]->cos_yearly += 0.28*RandN(RandGenerator);
		farms[i]->sin_6_month += 0.13*RandN(RandGenerator);
		farms[i]->cos_6_month += 0.12*RandN(RandGenerator);
		//~ farms[i]->cos_4_month = 0.0;
		//~ farms[i]->temp_eff_sq = 0.0;
		//~ farms[i]->rain_eff = 0.0;
		
		//Set the farms to their pre-outbreak readiness
		farms[i]->Detected = false; // Detected whether cases exist on farm
		farms[i]->MovementBanned = false; //Determines if the farm has been banned from moving animals
		farms[i]->ProtectionZone = false;
		farms[i]->SurveillanceZone = false;
		farms[i]->FreeArea = true;
		farms[i]->EverBeenInfected = false;
		farms[i]->FirstInfectedAnimalDueToMovement = false;
		farms[i]->S_cattle = farms[i]->number_of_cattle;
		farms[i]->S_sheep = farms[i]->number_of_sheep;
		for(int n = 0;n<NumOfInfStagesSheep;n++){
			farms[i]->I_sheep[n] = 0;
		}
		for(int n = 0;n<NumOfInfStagesCattle;n++){
			farms[i]->I_cattle[n] = 0;
		}
		farms[i]->R_sheep = 0;
		farms[i]->R_cattle = 0;
	}
}

void SolveFarmBitingPreference(double pref, double A){
	//Set the FarmBitinggrid to zero
	for(int i = 0; i<NumGridPointsMidgeS;i++){
		for(int j = 0; j<NumGridPointsMidgeE;j++){
			FarmBitingPreferenceGrid[i][j] = 0;
		}
	}	
	//Find the total weight of the farm population in the local midge grid square
	for(int k = 0;k<N;k++){
		FarmBitingPreferenceGrid[farms[k]->MidgeGrid_y][farms[k]->MidgeGrid_x] += exp(A*log(farms[k]->number_of_cattle + pref*farms[k]->number_of_sheep ));	
	}
	
	//Find the relative weight of each farm compared to total weight in the local midge grid square
	for(int k = 0;k<N;k++){
		farms[k]->RelLocalWeight = exp(A*log(farms[k]->number_of_cattle + pref*farms[k]->number_of_sheep ))/FarmBitingPreferenceGrid[farms[k]->MidgeGrid_y][farms[k]->MidgeGrid_x];	
		if(farms[k]->RelLocalWeight > 1 or farms[k]->RelLocalWeight < 0){std::cout<<"ERROR in local weight calculation at farm "<<k<<std::endl;}
	}
	
	//Find the inhomogeneous diffusion
	for(int i = 0; i<NumGridPointsMidgeS;i++){
		for(int j = 0; j<NumGridPointsMidgeE;j++){
			DiffusionGrid[i][j] = D_0/(1 + DiffMultScaler*FarmBitingPreferenceGrid[i][j]);
		}
	}	
}





void DiffusionWarning(){
	if(2*DT*D_0/(MidgeGridWidth*MidgeGridWidth) > 0.5){std::cout<<"DIFFUSION SOLUTION IS NOT ACCURATE ENOUGH!!!"<<std::endl;}
}

void InitialiseEpidemicAtCounty(){
	double x,y,x2,y2;
	int ChosenFarm;
	//Make list of farms in County
	std::vector<int> FarmsInCounty;
	for(int k = 0;k<N;k++){
		if(farms[k]->CountyNumber == OutbreakCounty){FarmsInCounty.push_back(k);}
	}
	std::cout<<"Number of farms in outbreak county = "<<FarmsInCounty.size()<<std::endl;
	//choose farm randomly
	double rand = double(FarmsInCounty.size())*RandU(RandGenerator);
	ChosenFarm = FarmsInCounty[int(floor(rand))];
	//Initialise outbreak by adding infectious midges local to the farm
	InfMidgeDensity[farms[ChosenFarm]->MidgeGrid_y][farms[ChosenFarm]->MidgeGrid_x] += InitialDensityOfInfectiousMidges;
	//Define centre point of grid chosen farm is in (metres BNG)
	//~ y = double(MidgeGridWidth*farms[ChosenFarm]->MidgeGrid_y + MidgeGridWidth/2);
	//~ x = double(MidgeGridWidth*farms[ChosenFarm]->MidgeGrid_x + MidgeGridWidth/2);
	x = farms[ChosenFarm]->x[0];
	y = farms[ChosenFarm]->x[1];
	//Add 10 infectious midges (on average) to each grid square with a centre within 25km
	for(int i = 0;i<NumGridPointsMidgeS;i++){
		for(int j = 0; j<NumGridPointsE;j++){
			y2 = double(MidgeGridWidth*i + MidgeGridWidth/2);
		    x2 = double(MidgeGridWidth*j + MidgeGridWidth/2);
		    if( (x-x2)*(x-x2) + (y-y2)*(y-y2) <= InitialWidthFromCentralSite*InitialWidthFromCentralSite ){
				InfMidgeDensity[i][j] += InitialDensityOfInfectiousMidges;
			}
		}
	}
		
    std::cout<<"Putting infectious midges into outbreak county "<<OutbreakCounty<<" around farm "<<ChosenFarm<<" at BNG coords: "<<farms[ChosenFarm]->x[0]<<"E, "<<farms[ChosenFarm]->x[1]<<"N"<<std::endl;
	#ifdef NOCONTROL
	std::cout<<"Starting outbreak without controls"<<std::endl; 
	#endif
	#ifdef TOTALBAN
	std::cout<<"Starting outbreak with total ban if BTV is detected"<<std::endl; 
	#endif
	#ifdef RESTRICTIONZONES
	std::cout<<"Starting outbreak with PZ/SZ controls"<<std::endl; 
	#endif
}


#endif

