
#ifndef CONST_PARAMETERS_H_INCLUDED
#define CONST_PARAMETERS_H_INCLUDED

//Parameters that remain fixed

const int N = 105028;//Number of farms
const int NumDays = 365;
const int NumOfReps = 5000;

//Initialisation parameters
int StartDayOfYear = 152;
double OutbreakCounty = 10; 
double InitialDensityOfInfectiousMidges = 10;
double InitialWidthFromCentralSite = 25000;

//Control Parameters

//~ double BanRadius = 0;
//~ double BanRadius = 1000;
//~ double BanRadius = 2000;
double BanRadius = 5000;

//~ double PZRadius = 0; 
//~ double SZRadius = 0;

double PZRadius = 50000;
double SZRadius = 100000;

//~ double PZRadius = 100000;
//~ double SZRadius = 150000;

//~ double PZRadius = 100000;
//~ double SZRadius = 300000;


//~ #define COUNTYBAN
//~ #define TOTALBAN
#define NOCONTROL
//~ #define NOFARMBAN

#define PREMOVEMENTTESTS
#ifndef TOTALBAN
	#define RESTRICTIONZONES
	#ifdef COUNTYBAN
		#undef COUNTYBAN
	#endif
#endif

#ifdef NOCONTROL
	#ifndef NOFARMBAN
		#define NOFARMBAN
	#endif
	#ifdef RESTRICTIONZONES
		#undef RESTRICTIONZONES
	#endif
	#ifdef COUNTYBAN
		#undef COUNTYBAN
	#endif
	#ifdef TOTALBAN
		#undef TOTALBAN
	#endif
	#ifdef PREMOVEMENTTESTS
		#undef PREMOVEMENTTESTS
	#endif
	#ifdef COUNTYBAN
		#undef COUNTYBAN
	#endif
#endif




//Epidemiological parameters
double P_V = 0.05;
double P_H = 0.9;
double SheepMortRate = 0.0055;
double DetectionProb_C = 0.0078;
double DetectionProb_S = 0.027;
double RecRateSheep = 0.06098;
double RecRateCattle = 0.04854;
double DiffusionLengthScale = 5000;
double D_0 = DiffusionLengthScale*DiffusionLengthScale;

//Initial condition

//Simulation Parameters
double DT=0.1;
double DTFarm = 0.1;
const int NumOfInfStagesSheep = 5;//14;
const int NumOfInfStagesCattle = 3;//5;
const int NumOfEIPStages = 5;
//~ #define VERBOSE
//~ #define DAILYREADOUT
#define PRINTOUTRESULTS

#define GENERATEPARAMETERSRANDOMLY
//~ #define USEDEFINEDPARAMETERS

#ifdef USEDEFINEDPARAMETERS
	#undef GENERATEPARAMETERSRANDOMLY
#endif	


//Grid parameters
double MidgeGridWidth = 5000;
double TempGridWidth = 5000;
double RainGridWidth = 25000;
double AutoCorrGridWidth = 5000;

double Discretisation_parameter = 0.0001;

// ****** PI ******
#define PI 3.14159265358979


//ENTOMOLOGICAL TEMPERATURE DEPENDENT RATES

//Biting rate per day with temp dependence
double BitingRate(double T){
	if(T>3.7 && T < 41.9){
	return 0.0002*T*(T - 3.7)*exp(0.37*log(41.9-T));
	}
    else{
		return 0;
	}	
}

//Temperature dependent mortality rate

double MortRate(double T){
	if(T > -2){
	return 0.009*exp(0.16*T);	
	}
	else{
		return 100;
	}
}

//Temperature dependent incubation rate
double IncubationRate(double T){
	double incub = 0.018*(T-13.4);
	if(incub > 0){
		return incub;
	}
	else{
		return 0;
	}

}

int IntMax(int a, int b){
	if(a > b){
		return a;
	}
	else{
		return b;
	}
}

int IntMin(int a, int b){
	if(a > b){
		return b;
	}
	else{
		return a;
	}
}

double dmod(double x,double y)
{
    if(fabs(x)<y/2){return fabs(x);}
    else{return (y - fabs(x)); }
    
}


#endif
