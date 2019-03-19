
#ifndef VARIABLE_PARAMETERS_H_INCLUDED
#define VARIABLE_PARAMETERS_H_INCLUDED
#include "RandomFuncs.h"
#include "const_parameters.h"

//Parameters that are drawn from a prior at each simulation

double PreferenceForSheep;
double TransmissionScalar;
double AttractPowScaler;
double DiffMultScaler;
double LHSParameters[NumOfReps][4];//Matrix for Latin hypercube samples

void DrawParamsFromPrior(const gsl_rng * r){
	PreferenceForSheep = RandGamma(r, 50.0,0.115/50.0);// (-1)*0.115*log(RandU(RandGenerator)); //Exponentially distributed with mean at 0.115 from Elbers et al experiment
	TransmissionScalar = 1 + RandU(RandGenerator)*9.0  ;// RandGamma(50,0.02);// (-1)*log(RandU(RandGenerator)); // exp(1)
	AttractPowScaler = RandU(RandGenerator); //Power scaling distributed U[0,1] implies prior mean attractiveness goes like (N-1)/ln(N)
	DiffMultScaler = (-0.1)*log(RandU(RandGenerator));//exp(1/0.1)
}

void PrintParameters(){
	std::cout<<PreferenceForSheep<<", "<<TransmissionScalar<<", "<<AttractPowScaler<<", "<<DiffMultScaler<<std::endl;
}
 

void DrawParamsFromLHSamples(int RepNumber){
	PreferenceForSheep = LHSParameters[RepNumber][0];
	TransmissionScalar = LHSParameters[RepNumber][1];
	AttractPowScaler = LHSParameters[RepNumber][2];
	DiffMultScaler = LHSParameters[RepNumber][3];
}

#endif
