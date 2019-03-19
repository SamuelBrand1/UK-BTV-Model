
#ifndef FARM_DEFN_H_INCLUDED
#define FARM_DEFN_H_INCLUDED
#include "const_parameters.h"
#include "Declarations.h"
#include "RandomFuncs.h"
#include "grids.h"
#include "variable_parameters.h"

void ImplementLocalMovementBan(int CentreFarm);
//Declaration of farm class

class farm{
	
public:
int ID;
int DayOfSim;
int DayOfYear;
double x[2]; //<------ Location of farm in BNG (metres)
double CountyNumber;  //CPR county number for farm
//Grid locations for each farm
int TempGrid_x;
int TempGrid_y;
int RainGrid_x;
int RainGrid_y;
int MidgeGrid_x;
int MidgeGrid_y;
int ACGrid_x;
int ACGrid_y;

//Group of Farm local random effects
double V_intercept;
double sin_yearly;
double cos_yearly;
double sin_6_month;
double cos_6_month;
double cos_4_month;
double temp_eff;
double temp_eff_sq;
double rain_eff;
double wind_eff;
double autocorr;
double overdispersion;
double ExtInfBiting;
	
//Farm demography
double number_of_sheep,number_of_cattle;
double S_sheep;
double I_sheep[NumOfInfStagesSheep];
double R_sheep;
double S_cattle;
double I_cattle[NumOfInfStagesCattle];
double R_cattle;

//Transmission risks
double RelLocalWeight; //The rel weight of the farm against other local farms.
double force;  // Rate at which susceptible hosts are infected
std::vector<std::vector<double> > movement_local_farms; // This is a vector list where each element is a non-zero link in contact matrix from farm, element 0 is the direction, element 1 is the risk

//Records of farm incidence
bool Detected; // Detected whether cases exist on farm
bool MovementBanned; //Determines if the farm has been banned from moving animals
bool ProtectionZone;
bool SurveillanceZone;
bool FreeArea;
bool EverBeenDetectedDuringRun;
bool EverBeenInfected; //Determines if the farm has ever had any infected animals
bool FirstInfectedAnimalDueToMovement; //Determines if the first infected animal on farm was due to movement onto the farm

//Each Days' weather effects
double TempToday;
double MeanRainLastWeek;
double WindToday;

//Faster sim stuff
std::vector<int> LocalFarmsID;

//FUNCTIONS FOR LOCAL COUNTS

double NumOfCattle(){
	double sum = 0;
	for(int i=0;i<NumOfInfStagesCattle;i++)
		{
		 sum += I_cattle[i];
		}
	return sum + S_cattle + R_cattle;
}

double NumOfSheep(){
	double sum = 0;
	for(int i=0;i<NumOfInfStagesSheep;i++){
	 sum += I_sheep[i];
	}
	return sum + S_sheep + R_sheep;
}

	 
double EffNumOfAnimals(double pref){
	return NumOfCattle() +  pref*NumOfSheep();
}
	 
double EffNumOfInfAnimals(double pref){
	double sum = 0;
	for(int i=0;i<NumOfInfStagesSheep;i++){
		sum += pref*I_sheep[i];
	}
	for(int i=0;i<NumOfInfStagesCattle;i++){
		sum += I_cattle[i];
	}
	return sum;
}

double NumOfInfCattle(){
	double sum = 0;
	for(int i=0;i<NumOfInfStagesCattle;i++){
		 sum += I_cattle[i];
	}
	return sum;
}



double NumOfInfSheep(){
	double sum = 0;
	for(int i=0;i<NumOfInfStagesSheep;i++){
		sum += I_sheep[i];
	}
	return sum;
}

bool SimulateFarmForDay(){
	if(EffNumOfInfAnimals(1) > 0){
		return true;
	}
	else{
		return false;
	}
}

//FUNCTIONS FOR EPIDEMIC EVENTS
	 
// Perform all the events that occur over a day
	 
void AnimalDeathsAndRecoveries(const gsl_rng * r){
	double stages;
	//~ //Step forward animal recoveries (using ~ hourly 0.04 days^{-1} timestep)
	
	//~ //SHEEP
	if(NumOfInfSheep()>0){
	//Time-step forwards
	stages = double(NumOfInfStagesSheep);
	int X;
	
	for(int t =0; double(t*DTFarm) < 1;t++){
		X = IntMin(RandPoisson(r,DTFarm*stages*RecRateSheep*I_sheep[NumOfInfStagesSheep-1]),int(I_sheep[NumOfInfStagesSheep-1]));
		I_sheep[NumOfInfStagesSheep-1] -= double(X);
		R_sheep += double(X);
		//Mortality
		X = IntMin(RandPoisson(r,DTFarm*SheepMortRate*I_sheep[NumOfInfStagesSheep-1]),int(I_sheep[NumOfInfStagesSheep-1]));
		if(X > 0 && !Detected){Detected = true;		NumOfFarmsDetectedToday++;
			#ifndef NOCONTROL
			#ifndef NOFARMBAN
			MovementBanned = true;
			#endif 
			ImplementLocalMovementBan(ID);
				if(!BTVObserved){
					#ifdef VERBOSE
						std::cout<<"BTV DETECTED!!! (by sheep death)"<<std::endl;
					#endif
					BTVObserved = true;
				}  
			#endif  
			} //Farm detection due to sheep death due to BT
		I_sheep[NumOfInfStagesSheep-1] -= X;
		NumOfSheepDeaths +=X;
		
		for(int n = NumOfInfStagesSheep-2;n>=0;n--){
			X = IntMin(RandPoisson(r,DTFarm*stages*RecRateSheep*I_sheep[n]),int(I_sheep[n]));
			I_sheep[n] -=X;
			I_sheep[n+1] +=X;	
		    X = IntMin(RandPoisson(r,DTFarm*SheepMortRate*I_sheep[n]),int(I_sheep[n]));
			if(X > 0 && !Detected){Detected = true;NumOfFarmsDetectedToday++;
				#ifndef NOCONTROL

				#ifndef NOFARMBAN
				MovementBanned = true;
				#endif 
				ImplementLocalMovementBan(ID);
				if(!BTVObserved){
					#ifdef VERBOSE
						std::cout<<"BTV DETECTED!!! (by sheep death)"<<std::endl;
					#endif
					BTVObserved = true;
				} 
				#endif  
				} //Farm detection due to sheep death due to BT
			I_sheep[n] -=X;
			NumOfSheepDeaths +=X;
			}
		}
	}
	
	
	//CATTLE
	if(NumOfInfCattle()>0){
		//Time-step forwards
		stages = double(NumOfInfStagesCattle);
		int X;
		for(int t =0; double(t*DTFarm) < 1;t++){
			X = IntMin(RandPoisson(r,DTFarm*stages*RecRateCattle*I_cattle[NumOfInfStagesCattle-1]),int(I_cattle[NumOfInfStagesCattle-1]));
			I_cattle[NumOfInfStagesCattle-1] -= double(X);
			R_cattle += double(X);
			for(int n = NumOfInfStagesCattle-2;n>=0;n--){
				X = IntMin(RandPoisson(r,DTFarm*stages*RecRateSheep*I_cattle[n]),int(I_cattle[n]));
				I_cattle[n] -=X;
				I_cattle[n+1] +=X;	
			}
		}
	}
	
	//Detection if not already detected

	if(Detected == false){
	double CattleNotDetectionProb = exp(NumOfInfCattle()*log(1 - DetectionProb_C));
	double SheepNotDetectionProb = exp(NumOfInfSheep()*log(1 - DetectionProb_S));
	double rand = RandU(RandGenerator);
	//See if detected and ban movement if so
	if(rand <= (1 - (CattleNotDetectionProb*SheepNotDetectionProb))){
		Detected = true;
		NumOfFarmsDetectedToday++;
		#ifndef NOCONTROL
		#ifndef NOFARMBAN
		MovementBanned = true;
		#endif 		
		ImplementLocalMovementBan(ID);
		if(!BTVObserved){
			#ifdef VERBOSE
				std::cout<<"BTV DETECTED!!!"<<std::endl;
			#endif
			BTVObserved = true;
			FirstDetectedFarmID = ID;
		}
		#endif
		}
    }
     
}

void GetWeatherToday(){
	TempToday = TempGrid[TempGrid_y][TempGrid_x][SimulationDay];
	WindToday = 0;
	MeanRainLastWeek = RainGrid[RainGrid_y][RainGrid_x][SimulationDay];
	autocorr = 0.0;//ACGrid[ACGrid_y][ACGrid_x];
	overdispersion = (1.08 + 0.3763)*RandN(RandGenerator);
}

 //Calculate condition average number of infected midges
 
void TransmissionHostsToMidges(){
	//Expected number of bites per Host
	
	double MeanNumOfInfectedMidges, MeanNumBitesPerAnimal;
	double doy = double(SimulationDay);
	double climate_component;
	//~ if(SimulationDay%365 >60 && SimulationDay%365 <330){
		//~ climate_component = V_intercept; //Base
		//~ climate_component += sin_yearly*sin(2*PI*doy/365.25) + cos_yearly*cos(2*PI*doy/365.25); //Yearly variation
		//~ climate_component += sin_6_month*sin(4*PI*doy/365.25) + cos_6_month*cos(4*PI*doy/365.25);//6 month variation
		//~ climate_component += cos_4_month*cos(6*PI*doy/365.25);//4 month variation
		//~ climate_component += temp_eff*TempToday + temp_eff_sq*TempToday*TempToday; //Temperature driven abundance
		//~ climate_component += overdispersion + autocorr;  //Overdisperion and temporal autocorrelation
	
		//~ //Get expected number  biting insects from the general population per animal
		//~ MeanNumBitesPerAnimal = TransmissionScalar*exp(climate_component);
		//~ if(MeanNumBitesPerAnimal > 5000){MeanNumBitesPerAnimal = 5000;}
	//~ }
	//~ else{
		//~ MeanNumBitesPerAnimal = 0;
	//~ }
	if(SimulationDay%365 >60 && SimulationDay%365 <330){
	climate_component = V_intercept; //Base
	climate_component += sin_yearly*sin(2*PI*doy/365.25) + cos_yearly*cos(2*PI*doy/365.25); //Yearly variation
	climate_component += sin_6_month*sin(4*PI*doy/365.25) + cos_6_month*cos(4*PI*doy/365.25);//6 month variation
	climate_component += cos_4_month*cos(6*PI*doy/365.25);//4 month variation
	climate_component += temp_eff*TempToday + temp_eff_sq*TempToday*TempToday; //Temperature driven abundance
	climate_component += overdispersion + autocorr;  //Overdisperion and temporal autocorrelation

	//Get expected number  biting insects from the general population per animal
	MeanNumBitesPerAnimal = TransmissionScalar*exp(climate_component);
	if(MeanNumBitesPerAnimal > 5000){MeanNumBitesPerAnimal = 5000;}
	}
	else{
		MeanNumBitesPerAnimal = 0;
	}
	
	MeanNumOfInfectedMidges = P_V*EffNumOfInfAnimals(PreferenceForSheep)*MeanNumBitesPerAnimal; //Mean number midges inoculated
	//Introduce new latent vectors
	LatentMidgeDensity[MidgeGrid_y][MidgeGrid_x][0] +=  MeanNumOfInfectedMidges;
}
	

	 //~ //Number of sucessfully infectious bites on animals
void TransmissionMidgesToHosts(const gsl_rng * r){
	double biting_rate = BitingRate(TempToday);
	double BitingProb = (1 - exp((-1)*biting_rate));
	force = RelLocalWeight*InfMidgeDensity[MidgeGrid_y][MidgeGrid_x]*BitingProb;
	int A,B;
	double ProbBitePerSheep = PreferenceForSheep/EffNumOfAnimals(PreferenceForSheep);
	double ProbBitePerCattle = 1/EffNumOfAnimals(PreferenceForSheep);
	double ProbInfectionPerSheep = 1 - exp((-1)*force*ProbBitePerSheep*P_H );
	double ProbInfectionPerCattle = 1 - exp((-1)*force*ProbBitePerCattle*P_H );
	//~ //Use poisson approximation if appropriate
	if((S_sheep > 100 && ProbInfectionPerSheep < 0.01) && S_sheep*ProbInfectionPerSheep < 20){
		A = IntMin(RandPoisson(r,S_sheep*ProbInfectionPerSheep),int(S_sheep));
	}
	else{
		A = RandBinomial(r,int(S_sheep),ProbInfectionPerSheep);
	}
	
	if((S_cattle > 100 && ProbInfectionPerCattle < 0.01) && S_cattle*ProbInfectionPerCattle < 20){
		B = IntMin(RandPoisson(r,S_cattle*ProbInfectionPerCattle),int(S_cattle));
	}
	else{
		B = RandBinomial(r,int(S_cattle),ProbInfectionPerCattle);
	}

	S_sheep -= A; I_sheep[0] += A; NumberOfSheepInfectedToday += A;
	S_cattle -= B; I_cattle[0] +=B; NumberOfCattleInfectedToday += B;
}




	    //~ //FUNCTIONS FOR PRINTING

void PrintState(){
	std::cout<<"FARM "<<ID<<" on day of year "<<DayOfYear%365 + 1<<" ("<<DayOfSim<<")"<<std::endl;
	std::cout<<"S_C = "<<S_cattle<<", I_C = "<<NumOfInfCattle()<<", R_C = "<<R_cattle<<std::endl;
	std::cout<<"S_S = "<<S_sheep<<", I_S = "<<NumOfInfSheep()<<", R_S = "<<R_sheep<<std::endl;	
	std::cout<<"Temp = "<<TempToday<<", mean rainfall = "<<MeanRainLastWeek<<std::endl;					
}



};	

//Declare and array of pointers to the farm types
farm* farms[N];

double DistSqBetweenFarms(int ID1,int ID2){
	return (farms[ID1]->x[0] - farms[ID2]->x[0])*(farms[ID1]->x[0] - farms[ID2]->x[0]) + (farms[ID1]->x[1] - farms[ID2]->x[1])*(farms[ID1]->x[1] - farms[ID2]->x[1]);
}

void FindLocalFarms(int CentreFarmID){
	for(int k = 0; k<N ; k++){
		if(k != CentreFarmID && DistSqBetweenFarms(k,CentreFarmID) < BanRadius*BanRadius){
			farms[CentreFarmID]->LocalFarmsID.push_back(k);
		}
	}
}


#endif


