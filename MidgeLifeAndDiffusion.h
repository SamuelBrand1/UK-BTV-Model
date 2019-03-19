#ifndef MIDGELIFEANDDIFFUSION_H_INCLUDED
#define MIDGELIFEANDDIFFUSION_H_INCLUDED

#include "const_parameters.h"
#include "grids.h"

int R = int(MidgeGridWidth/TempGridWidth); // Useful if midge grid is finer grained than temperature grid

void MidgeMortalityAndIncubation(){
	//Reduce the mean number of Midges according to the temperature
	double TempHereToday,mort,incub;
	double SolnVector[NumOfEIPStages+1];
	double sum;
	for(int i = 0; i< NumGridPointsMidgeS ; i++){
		for(int j = 0; j<NumGridPointsMidgeE;j++){
			TempHereToday =  TempGrid[i*R][j*R][SimulationDay];
			//~ if(i == 46 && j == 100){TempHereToday = 20;}
			mort = exp((-1)*MortRate(TempHereToday));
			incub = double(NumOfEIPStages*IncubationRate(TempHereToday));

			//Mortality and check if EIP calculation is necessary
			InfMidgeDensity[i*R][j*R] *= mort;
			sum = 0;
			for(int k = 0;k<NumOfEIPStages;k++){
				LatentMidgeDensity[i*R][j*R][k] *= mort;
				sum += LatentMidgeDensity[i*R][j*R][k];
			}

			if(incub >0 && sum > 0){
				//Calculate new distribution of latent vectors
				//Transitions from an earlier EIP stage to a later one
				for(int n=0;n < NumOfEIPStages;n++){
					SolnVector[n] = 0;
					for(int k=0;k<=n;k++){
						SolnVector[n] += LatentMidgeDensity[i*R][j*R][k]*PoissonPmf(n-k,incub);
					} 
				}
				//Transitions to infectious state
				SolnVector[NumOfEIPStages] = InfMidgeDensity[i*R][j*R];
				for(int k = 0; k<NumOfEIPStages;k++)
				{
					SolnVector[NumOfEIPStages]+= LatentMidgeDensity[i*R][j*R][k]*PoissonQ(NumOfEIPStages - k - 1,incub);//Probability of getting at least as many transistions as required
				}
				//Resolve the solution
				for(int n =0 ; n< NumOfEIPStages;n++){
					LatentMidgeDensity[i*R][j*R][n] = SolnVector[n];
				}
				InfMidgeDensity[i*R][j*R] = SolnVector[NumOfEIPStages];
			}
		}
	}
} 

void MidgeDiffusion(){
	double D;
	//Implement diffusion for each stage of vector EIP
	for(int k = 0;k<NumOfEIPStages;k++){
		//Calculate transition rates
		for(int i = 1; i< NumGridPointsMidgeS - 1 ; i++){
			for(int j = 1; j<NumGridPointsMidgeE - 1;j++){
				if(LatentMidgeDensity[i][j][k] > 0.00001){
				D = DiffusionGrid[i][j];			
				DiffusionSolnGrid[i][j] -= 2*D*DT*LatentMidgeDensity[i][j][k]/(MidgeGridWidth*MidgeGridWidth);
				DiffusionSolnGrid[i+1][j] += 0.5*D*DT*LatentMidgeDensity[i][j][k]/(MidgeGridWidth*MidgeGridWidth);
				DiffusionSolnGrid[i-1][j] += 0.5*D*DT*LatentMidgeDensity[i][j][k]/(MidgeGridWidth*MidgeGridWidth);
				DiffusionSolnGrid[i][j+1] += 0.5*D*DT*LatentMidgeDensity[i][j][k]/(MidgeGridWidth*MidgeGridWidth);
				DiffusionSolnGrid[i][j-1] += 0.5*D*DT*LatentMidgeDensity[i][j][k]/(MidgeGridWidth*MidgeGridWidth);
				}
			}
		}
				//Implement solution with discretisation
		for(int i = 1; i< NumGridPointsMidgeS - 1 ; i++){
			for(int j = 1; j<NumGridPointsMidgeE - 1;j++){
				LatentMidgeDensity[i][j][k] += DiffusionSolnGrid[i][j];
				DiffusionSolnGrid[i][j] = 0;//Reset grid
			}
		}		
	}
	// And for infectious vectors
	for(int i = 1; i< NumGridPointsMidgeS - 1 ; i++){
		for(int j = 1; j<NumGridPointsMidgeE - 1;j++){
			if(InfMidgeDensity[i][j]>  0.00001){
			D = DiffusionGrid[i][j];			
			DiffusionSolnGrid[i][j] -= 2*D*DT*InfMidgeDensity[i][j]/(MidgeGridWidth*MidgeGridWidth);
			DiffusionSolnGrid[i+1][j] += 0.5*D*DT*InfMidgeDensity[i][j]/(MidgeGridWidth*MidgeGridWidth);
			DiffusionSolnGrid[i-1][j] += 0.5*D*DT*InfMidgeDensity[i][j]/(MidgeGridWidth*MidgeGridWidth);
			DiffusionSolnGrid[i][j+1] += 0.5*D*DT*InfMidgeDensity[i][j]/(MidgeGridWidth*MidgeGridWidth);
			DiffusionSolnGrid[i][j-1] += 0.5*D*DT*InfMidgeDensity[i][j]/(MidgeGridWidth*MidgeGridWidth);
			}
		}
	}
			//Implement solution with discretisation
	for(int i = 1; i< NumGridPointsMidgeS - 1 ; i++){
		for(int j = 1; j<NumGridPointsMidgeE - 1;j++){
			InfMidgeDensity[i][j] += DiffusionSolnGrid[i][j];
			DiffusionSolnGrid[i][j] = 0;//Reset grid
		}
	}	
	
		
}

void MidgeDiffusionForADay(){
	double DayTime =0;
	while(DayTime < 1){
	MidgeDiffusion();
	DayTime += DT;
	}
}

#endif

