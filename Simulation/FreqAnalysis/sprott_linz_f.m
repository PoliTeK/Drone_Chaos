%% Sound parameters
t_sound = 3;
Fs = 44800;

%% Sprott-Linz F system
function trajectory = sprott_linz_f_attractor(start_point, end_time, alpha)
    % Define the system of equations
    lorenz_eq = @(t, y) [y(2) + y(3);
                         -y(1) + alpha*y(2);
                         y(1)^2 - y(3)];
    
    % Set the time span for the simulation
    tspan = [0, end_time];
    
    % Use ode45 to solve the system of equations
    [t, y] = ode45(lorenz_eq, tspan, start_point);
    
    % Return the trajectory
    trajectory = struct('time', t, 'states', y);
end

% Define parameters for the Lorenz attractor
start_point = [0.1; 0; 0]; % Initial conditions
end_time = 8000; % End time for the simulation
alpha = 0.5; % Model parameter (0.2 < alpha < 0.51)

% Call the lorenz_attractor function
trajectory = sprott_linz_f_attractor(start_point, end_time, alpha);
trajectory.states(:,2) = highpass(trajectory.states(:,2), 0.01);

%% Plot the obtained trajectory
figure;
plot3(trajectory.states(:,1), trajectory.states(:,2), trajectory.states(:,3));
xlabel('X-axis');
ylabel('Y-axis');
zlabel('Z-axis');
title('Sprott-Linz F Attractor Trajectory');
grid on;

%% Frequency analysis
% Compute the Fourier transforms of the absolute values of the trajectory coordinates
f_x = fftshift(abs(fft(trajectory.states(:,1)))) / length(trajectory.states(:,1));
f_y = fftshift(abs(fft(trajectory.states(:,2)))) / length(trajectory.states(:,2));
f_z = fftshift(abs(fft(trajectory.states(:,3)))) / length(trajectory.states(:,3));

% Define frequency axis
n = length(trajectory.states(:,1));
f = (-n/2:n/2-1)*(1/end_time); % Frequency vector

% Plot the Fourier transforms
figure;
subplot(3,1,1);
plot(f, f_x);
xlabel('Frequency');
ylabel('Magnitude');
title('Fourier Transform of X Coordinate (filtered)');
grid on;

subplot(3,1,2);
plot(f, f_y);
xlabel('Frequency');
ylabel('Magnitude');
title('Fourier Transform of Y Coordinate');
grid on;

subplot(3,1,3);
plot(f, f_z);
xlabel('Frequency');
ylabel('Magnitude');
title('Fourier Transform of Z Coordinate');
grid on;

%% Resample for sound
trajectory.realtime = 0:1/Fs:t_sound;
trajectory.resampled = spline(trajectory.time, trajectory.states(:,1), trajectory.realtime / t_sound * end_time);

%% Play sound
soundsc(trajectory.resampled, Fs)

%% Other
plot(trajectory.time(1:1000), trajectory.states(1:1000,1))