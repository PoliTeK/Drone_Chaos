% Parameters
a = 1.33;
b = 0.333;
N = 44100;  % 1 second of audio at 44.1kHz
Fs = 44100; % Sampling rate

% Generate Hénon coordinates
x = zeros(1, N);
y = zeros(1, N);
x(1) = 0.1;
y(1) = 0.3;

for n = 2:N
    x(n) = 1 - a * x(n-1)^2 + y(n-1);
    y(n) = b * x(n-1);
end

% Normalize to [-1, 1]
x_norm = 2 * (x - min(x)) / (max(x) - min(x)) - 1;
y_norm = 2 * (y - min(y)) / (max(y) - min(y)) - 1;

% Combine into stereo audio
audio = [x_norm; y_norm]';  % Transpose to (N x 2)

% Play sound
sound(audio, Fs);

% Optional: save to .wav
audiowrite('henon_attractor_audio.wav', audio, Fs);

% Plot for reference
figure;
plot(x_norm, y_norm, '.k', 'MarkerSize', 1);
title('Normalized Hénon Attractor');
xlabel('x'); ylabel('y');
axis equal;