%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Script per inserire gli input dell'attrattore di Lorenz
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Posizione iniziale
x0 = 0.01;
y0 = 0.1;
z0 = 0;

% Parametri del sistema (standard: rho = 28, sigma = 10, beta = 8/3)
rho = 25;
sigma = 12;
beta = 3;

% Parametri di integrazione
T_max = 100; % Tempo massimo della simulazione
dt = 0.01; % Passo di integrazione

% Lancia la simulazione e plotta il risultato
trajectory = lorenz_attractor(rho,sigma,beta,x0,y0,z0,T_max,dt);