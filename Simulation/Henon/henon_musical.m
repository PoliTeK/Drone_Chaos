% Parameters
a = 1.4;
b = 0.3;
N = 200;      % Number of notes (keep small for faster playback)
Fs = 44100;   % Sample rate
note_duration = 0.2;  % Duration of each note in seconds

% Generate Hénon map
x = zeros(1, N);
y = zeros(1, N);
x(1) = 0.1;
y(1) = 0.3;

for n = 2:N
    x(n) = 1 - a * x(n-1)^2 + y(n-1);
    y(n) = b * x(n-1);
end

% Normalize x to pitch range (e.g., MIDI notes 60–84 = C4–C6)
x_scaled = rescale(x, 60, 84);  % MIDI range
frequencies = 440 * 2.^((x_scaled - 69)/12);  % Convert to Hz

% Normalize y to amplitude
amplitudes = rescale(abs(y), 0.1, 0.9);

% Create melody
t = linspace(0, note_duration, round(Fs * note_duration));
melody = [];

for i = 1:N
    f = frequencies(i);
    A = amplitudes(i);
    note = A * sin(2 * pi * f * t);
    melody = [melody, note];
end

% Play and save
sound(melody, Fs);
%audiowrite('henon_melody.wav', melody, Fs);

% plot frequency over time
figure;
plot(frequencies, '.-');
xlabel('Note Index');
ylabel('Frequency (Hz)');
title('Hénon Attractor → Pitch Mapping');
