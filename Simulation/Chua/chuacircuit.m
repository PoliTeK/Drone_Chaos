
clc; clear; close all;

alpha = 18.39; %%classico 16.6, a 18.4 diventa uno spirale
beta = 39; %% classico è 28, 
m0 = -1.143;
m1 = -0.714;


chua_diode = @(x) m1*x + 0.5*(m0 - m1)*(abs(x + 1) - abs(x - 1));

chua_ode = @(t, X) [
    alpha * (X(2) - X(1) - chua_diode(X(1)));
    X(1) - X(2) + X(3);
    -beta * X(2)
];

X0 = [0.1; 0; 0];

tspan = [0 100];

[t, X] = ode45(chua_ode, tspan, X0);

figure;
plot3(X(:,1), X(:,2), X(:,3), 'k');
xlabel('x'); ylabel('y'); zlabel('z');
title('Chua''s Circuit - Double Scroll Attractor');
grid on; axis tight; view(35, 20);
