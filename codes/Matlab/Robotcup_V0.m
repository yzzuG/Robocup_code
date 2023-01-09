close all
clear all
clc

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% PARAMETRES

% parametres mecaniques
% rapport de reduction entre la poulie du moteur et celle de la roue
reduction_roues = 2.75;

% rayon de la roue
roues = 0.05;           % en m

% parametres elctriques moteurs
% coeficient entre vitesse et tension dalimention sur fil blanc moteur
scaling = 31.42;        % rad/s/V

% tension maximum
u_max = 10;             % en V


%% PARAMETRES SIMU

T_final = 22;       % en s

Te = 0.100;         % en s

vcap = 3.2;
alpha_cap = 30;

robocup_simu;
%% CALCULS
figure
hold on
grid on
%for i=0:Te:T_final
    sim('robocup_simu',T_final);
    plot(x_r,y_r);

%end

%% SIMULATION
