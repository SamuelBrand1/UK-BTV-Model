#ifndef GRIDS_H_INCLUDED
#define GRIDS_H_INCLUDED

#include "const_parameters.h"
#include "Declarations.h"

//Declare grid for Temperature for UK --- 5x5 km square orientated as a matrix with SW corner at 0,0 BNG is also the [0][0] element of each array. 
//~ Additional elements modulo d1*d2 are extra days starting from 1st Jan 2007

const int NumGridPointsS = 244;
const int NumGridPointsE = 131;
double TempGrid[NumGridPointsS][NumGridPointsE][NumDays];

//Declare grid for mean rainfall over last week for UK --- 25x25 km square orientated as a matrix with SW corner at 0,0 BNG is also the [0][0] element of each array. 
//~ Additional elements modulo d1*d2 are extra days starting from 1st Jan 2007

const int NumGridPointsRainS = 50;
const int NumGridPointsRainE = 27;
double RainGrid[NumGridPointsRainS][NumGridPointsRainE][NumDays];

//Declare grids for the diffusive midges

const int NumGridPointsMidgeS = 244;
const int NumGridPointsMidgeE = 131;
double LatentMidgeDensity[NumGridPointsMidgeS][NumGridPointsMidgeE][NumOfEIPStages];
double InfMidgeDensity[NumGridPointsMidgeS][NumGridPointsMidgeE];
double FarmBitingPreferenceGrid[NumGridPointsMidgeS][NumGridPointsMidgeE];
double DiffusionSolnGrid[NumGridPointsMidgeS][NumGridPointsMidgeE];
double DiffusionGrid[NumGridPointsMidgeS][NumGridPointsMidgeE];
//Declare grid for Autocorrelation

const int NumGridPointsACS = 244;
const int NumGridPointsACE = 131;
double ACGrid[NumGridPointsACS][NumGridPointsACE];

void PrintGridSquareData(int y,int x){
	double sum =0;
	for(int n = 0;n<NumOfEIPStages;n++){
		sum += LatentMidgeDensity[y][x][n];
	}
	std::cout<<"t ("<<x<<", "<<y<<"): Latent midge density = "<<sum<<", Infectious Midge density = "<<InfMidgeDensity[y][x]<<std::endl; 
}

void DetailedPrintGridSquareData(int y,int x){
	std::cout<<"t ("<<x<<", "<<y<<"): Latent midge density = (";
	for(int n = 0;n<NumOfEIPStages;n++){
		std::cout<<LatentMidgeDensity[y][x][n]<<", ";
	}
	std::cout<<std::endl;
	std::cout<<"Infectious Midge density = "<<InfMidgeDensity[y][x]<<std::endl; 
	std::cout<<"temp = "<<TempGrid[y][x][SimulationDay]<<std::endl;
}

void PrintInfMidgeGridOutToFile(){
	char buffer[100];
	std::stringstream filenamess; 
	filenamess.clear();
	filenamess.str("");
	filenamess<<"./IMAGE/InfMidgeImageDay_"<<SimulationDay<<".txt";
	filenamess>>buffer;
	std::ofstream out(buffer);
	for(int i = 0; i<NumGridPointsMidgeS;i++){
		for(int j=0; j<NumGridPointsMidgeE;j++){
			out<<InfMidgeDensity[i][j]<<", ";
		}
		out<<std::endl;
	}
	out.close();
}
#endif
