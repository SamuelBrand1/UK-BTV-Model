#ifndef RANDOMFUNCS_H_INCLUDED
#define RANDOMFUNCS_H_INCLUDED
typedef std::chrono::high_resolution_clock myclock;
myclock::time_point beginning = myclock::now();

myclock::duration d = myclock::now() - beginning;
unsigned seed1 = d.count();

std::mt19937_64 RandGenerator (seed1); // minstd_rand0 is a standard linear_congruential_engine
//Declaration of functions producing random variables with specified parameters and standard random variables
#include "Declarations.h"
const gsl_rng_type * T;
gsl_rng * r;

std::normal_distribution<double> RandN(0,1.0);
std::uniform_real_distribution<double> RandU(0.0,1.0);

int RandBinomial(const gsl_rng * r, int n, double p){
  //~ std::binomial_distribution<int> distribution(n,p);
  //~ return distribution(RandGenerator);
  //~ int sum =0;
  //~ for(int i = 0;i<n;i++){
	  //~ if(RandU(RandGenerator)<=p){sum++;}
  //~ }
  return gsl_ran_binomial ( r,  p,  n);
;
}

int RandPoisson(const gsl_rng * r, double lambda){
	//~ std::poisson_distribution<int> distribution(lambda);
	//~ return distribution(RandGenerator);
	//~ int sum = 0;
	//~ double X = (-1)*log(RandU(RandGenerator));
	//~ while(X<lambda){
		//~ sum++;
		//~ X += (-1)*log(RandU(RandGenerator));
	//~ }
	return gsl_ran_poisson (r, lambda);
}


double PoissonPmf(int x, double lambda){
	//~ if(lambda > 0){
	//~ return exp((-1)*lambda + double(x*log(lambda)) )/tgamma(x+1);
	//~ }
	//~ else{
		//~ if(x == 0){return 1;}
		//~ else{return 0;}
	//~ }
	return gsl_ran_poisson_pdf (x, lambda);
}

double PoissonF(int x, double lambda){
	//~ double sum = 0;
	//~ for(int k = 0; k<=x ;k++){
		//~ sum += PoissonPmf(k,lambda);
	//~ }
	return gsl_cdf_poisson_P (x, lambda);
}

double PoissonQ(int x, double lambda){
	return gsl_cdf_poisson_Q (x, lambda);
}

double RandGamma(const gsl_rng * r, double shape, double scale){
	//~ std::gamma_distribution<double> distribution(shape,scale);
	//~ return distribution(RandGenerator);
	return gsl_ran_gamma (r,shape,scale);
}


int RandNegBin(const gsl_rng * r, double k, double p){
	//Uses Poisson gamma mixture
	//Wiki version of the parametrisation
	double G = RandGamma(r,k,p/(1-p));
	return RandPoisson(r,G);
}

#endif

