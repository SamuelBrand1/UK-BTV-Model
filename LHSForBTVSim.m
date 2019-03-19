%Script for creating a Latin hypercube sample for BTV modelling
X = lhsdesign(1000,4);

%Col 1: Sheep pref ~exp(1/0.115)
%Col 2: Transmission scaler ~exp(1)
%Col 3: Power multiplier for attractiveness ~U(0,1)
%Col 4: Scaler for Diffusion ~exp(1/0.001)

X(:,1) = -0.115*log(X(:,1));
X(:,2) = gaminv(X(:,2),50*ones(1000,1),0.02*ones(1000,1));
X(:,3) = X(:,3)*2;
X(:,4) = -0.001*log(X(:,4));

save LHSForBTVSims.txt X -ASCII


% PreferenceForSheep = (-1)*0.115*log(RandU(RandGenerator)); //Exponentially distributed with mean at 0.115 from Elbers et al experiment
% 	TransmissionScalar = RandGamma(50,0.02);// (-1)*log(RandU(RandGenerator)); // exp(1)
% 	AttractPowScaler = RandU(RandGenerator)*2; //Power scaling distributed U[0,1] implies prior mean attractiveness goes like (N-1)/ln(N)
% 	DiffMultScaler = (-0.001)*log(RandU(RandGenerator));//exp(1/0.001)