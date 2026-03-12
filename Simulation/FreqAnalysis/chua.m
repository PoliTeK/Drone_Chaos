clc; clear; close all;

t_max = 5;
Fs = 44800;
k = 1;

% p_max = 1/(2 * r_step * k);
r_step = 0.01;
r_max = t_max * Fs * k * r_step;

%%

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

r_span = [0 r_max];

[r, X] = ode45(chua_ode, r_span, X0);

%%

t_audio = 0:(1/Fs):t_max;
r_audio = t_audio * r_max / t_max;

x_audio = spline(r, X(:, 1), r_audio);
x_sound = diff(x_audio);

plot(t_audio(2:end), x_sound);

%%

N = length(x_sound);
Y = fftshift(fft(x_sound)/N);
f_axis = (-N/2:(N-1)/2) * 1/t_max;
plot(f_axis, abs(Y));


%%

figure;
plot3(X(:,1), X(:,2), X(:,3), 'k');
xlabel('x'); ylabel('y'); zlabel('z');
title('Chua''s Circuit - Double Scroll Attractor');
grid on; axis tight; view(35, 20);