%Script for plotting diffusion

I0 = load('AffectedFarmsImageDay_153.txt');
I1 = load('AffectedFarmsImageDay_190.txt');
I2 = load('AffectedFarmsImageDay_240.txt');
I3 = load('AffectedFarmsImageDay_270.txt');
I4 = load('AffectedFarmsImageDay_300.txt');
% I5 = load('AffectedFarmsImageDay_430.txt');
% I6 = load('AffectedFarmsImageDay_500.txt');
% I7 = load('AffectedFarmsImageDay_600.txt');
% I8 = load('AffectedFarmsImageDay_700.txt');

G0 = load('InfMidgeImageDay_153.txt');
G1 = load('InfMidgeImageDay_190.txt');
G2 = load('InfMidgeImageDay_240.txt');
G3 = load('InfMidgeImageDay_270.txt');
G4 = load('InfMidgeImageDay_300.txt');
% G5 = load('InfMidgeImageDay_430.txt');
% G6 = load('InfMidgeImageDay_500.txt');
% G7 = load('InfMidgeImageDay_600.txt');
% G8 = load('InfMidgeImageDay_700.txt');

UK = load('UKLandGrid.mat');
%%
figure(2)
clf

 

subplot(5,2,1)
imagesc(flipud(G0)./25)
colorbar

subplot(5,2,2)
contour((1:131)*5000,(1:244)*5000,UK.UKLand,[1 1])
hold on
scatter(I0(:,1),I0(:,2),50,sum(I0(:,[4,5,7,8]),2),'filled' )
hold off

subplot(5,2,3)
imagesc(flipud(G1)./25)
colorbar

subplot(5,2,4)
contour((1:131)*5000,(1:244)*5000,UK.UKLand,[1 1])
hold on
scatter(I1(:,1),I1(:,2),50,sum(I1(:,[4,5,7,8]),2),'filled' )
hold off

subplot(5,2,5)
imagesc(flipud(G2)./25)
colorbar

subplot(5,2,6)
contour((1:131)*5000,(1:244)*5000,UK.UKLand,[1 1])
hold on
scatter(I2(:,1),I2(:,2),50,sum(I2(:,[4,5,7,8]),2),'filled' )
hold off

subplot(5,2,7)
imagesc(flipud(G3)./25)
colorbar

subplot(5,2,8)
contour((1:131)*5000,(1:244)*5000,UK.UKLand,[1 1])
hold on
scatter(I3(:,1),I3(:,2),50,sum(I3(:,[4,5,7,8]),2),'filled' )
hold off


subplot(5,2,9)
imagesc(flipud(G4)./25)
colorbar

subplot(5,2,10)
contour((1:131)*5000,(1:244)*5000,UK.UKLand,[1 1])
hold on
scatter(I4(:,1),I4(:,2),5,sum(I4(:,[4,5,7,8]),2),'filled' )
hold off
% 
% subplot(5,2,11)
% imagesc(flipud(G5))
% colorbar
% 
% subplot(5,2,12)
% contour((1:131)*5000,(1:244)*5000,UK.UKLand,[1 1])
% hold on
% scatter(I5(:,1),I5(:,2),50,sum(I5(:,[4,5,7,8]),2),'filled' )
% hold off
% 
% 
% subplot(5,2,13)
% imagesc(flipud(G6))
% colorbar
% 
% subplot(5,2,14)
% contour((1:131)*5000,(1:244)*5000,UK.UKLand,[1 1])
% hold on
% scatter(I6(:,1),I6(:,2),50,sum(I6(:,[4,5,7,8]),2),'filled' )
% hold off
% 
% subplot(5,2,15)
% imagesc(flipud(G7))
% colorbar
% 
% subplot(5,2,16)
% contour((1:131)*5000,(1:244)*5000,UK.UKLand,[1 1])
% hold on
% scatter(I7(:,1),I7(:,2),50,sum(I7(:,[4,5,7,8]),2),'filled' )
% hold off
% 
% subplot(5,2,17)
% imagesc(flipud(G8))
% colorbar
% 
% subplot(5,2,18)
% contour((1:131)*5000,(1:244)*5000,UK.UKLand,[1 1])
% hold on
% scatter(I8(:,1),I8(:,2),50,sum(I8(:,[4,5,7,8]),2),'filled' )
% hold off
