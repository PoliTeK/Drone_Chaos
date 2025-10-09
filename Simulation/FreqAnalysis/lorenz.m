%% Sound parameters
Fs = 44800;
sound_time = 2.5;
dr = 0.01;

%%
function trajectory = lorenz_attractor(start_point, end_time, sigma, beta, rho)
    % Define the Lorenz system of equations
    lorenz_eq = @(t, y) [sigma * (y(2) - y(1)); 
                         y(1) * (rho - y(3)) - y(2); 
                         y(1) * y(2) - beta * y(3)];
    
    % Set the time span for the simulation
    tspan = [0, end_time];
    
    % Use ode45 to solve the system of equations
    [t, y] = ode45(lorenz_eq, tspan, start_point);
    
    % Return the trajectory
    trajectory = struct('time', t, 'states', y);
end

% Define parameters for the Lorenz attractor
start_point = [1; 1; 1]; % Initial conditions
end_time = Fs * sound_time * dr;            % End time for the simulation
sigma = 10;              % Standard deviation parameter
beta = 8/3;              % Beta parameter
rho = 28;                % Rho parameter

% Call the lorenz_attractor function
trajectory = lorenz_attractor(start_point, end_time, sigma, beta, rho);

% Plot the obtained trajectory
figure;
plot3(trajectory.states(:,1), trajectory.states(:,2), trajectory.states(:,3));
xlabel('X-axis');
ylabel('Y-axis');
zlabel('Z-axis');
title('Lorenz Attractor Trajectory');
grid on;

%% Frequency analysis
d_trajectory = struct('time', trajectory.time(2:end), 'states', diff(trajectory.states));

% Compute the Fourier transforms of the absolute values of the trajectory coordinates
f_x = fftshift(abs(fft(trajectory.states(:,1))));
f_y = fftshift(abs(fft(trajectory.states(:,2))));
f_z = fftshift(abs(fft(trajectory.states(:,3))));

f_dx = fftshift(abs(fft(d_trajectory.states(:,1))));
f_dy = fftshift(abs(fft(d_trajectory.states(:,2))));
f_dz = fftshift(abs(fft(d_trajectory.states(:,3))));

% Define frequency axis
n = length(trajectory.states(:,1));
f = (-n/2:n/2-1)*(1/end_time); % Frequency vector
n_d = length(d_trajectory.states(:,1));
f_d = (-n_d/2:n_d/2-1)*(1/end_time); % Frequency vector

% Plot the Fourier transforms
figure;
subplot(3,2,1);
plot(f, f_x);
xlabel('Frequency');
ylabel('Magnitude');
title('Fourier Transform of X Coordinate');
grid on;

subplot(3,2,2);
plot(f_d, f_dx);
xlabel('Frequency');
ylabel('Magnitude');
title('Fourier Transform of sX Coordinate');
grid on;

subplot(3,2,3);
plot(f, f_y);
xlabel('Frequency');
ylabel('Magnitude');
title('Fourier Transform of Y Coordinate');
grid on;

subplot(3,2,4);
plot(f_d, f_dy);
xlabel('Frequency');
ylabel('Magnitude');
title('Fourier Transform of sY Coordinate');
grid on;

subplot(3,2,5);
plot(f, f_z);
xlabel('Frequency');
ylabel('Magnitude');
title('Fourier Transform of Z Coordinate');
grid on;

subplot(3,2,6);
plot(f_d, f_dz);
xlabel('Frequency');
ylabel('Magnitude');
title('Fourier Transform of sZ Coordinate');
grid on;

%% Resample for sound
d_trajectory.realtime = 0:1/Fs:sound_time;
d_trajectory.resampled = spline(d_trajectory.time, d_trajectory.states(:,2), d_trajectory.realtime / sound_time * end_time);

trajectory.realtime = 0:1/Fs:sound_time;
trajectory.resampled = spline(trajectory.time, trajectory.states(:,2), trajectory.realtime / sound_time * end_time);

%% Play sound
soundsc(trajectory.resampled, Fs);