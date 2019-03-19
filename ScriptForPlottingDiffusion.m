%Script for plotting diffusion
figure
I1 = load('InfMidgeImageDay_153.txt');
I2 = load('InfMidgeImageDay_190.txt');
I3 = load('InfMidgeImageDay_240.txt');
I4 = load('InfMidgeImageDay_270.txt');
I5 = load('InfMidgeImageDay_300.txt');
subplot(2,3,1)
imagesc(flipud(I1))
subplot(2,3,2)
imagesc(flipud(I2))
subplot(2,3,3)
imagesc(flipud(I4))
imagesc(flipud(I3))
subplot(2,3,4)
imagesc(flipud(I4))
subplot(2,3,5)
imagesc(flipud(I5))
