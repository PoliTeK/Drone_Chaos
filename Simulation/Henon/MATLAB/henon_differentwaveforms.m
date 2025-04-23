% Settings
waveform = 'saw';  % Options: 'sine', 'square', 'triangle', 'saw'
a = 1.4; b = 0.3; N = 200; Fs = 44100; note_duration = 0.2;

% Generate Hénon map
x = zeros(1, N); y = zeros(1, N); x(1) = 0.1; y(1) = 0.3;
for n = 2:N
    x(n) = 1 - a * x(n-1)^2 + y(n-1);
    y(n) = b * x(n-1);
end

% Convert x to MIDI pitch, then to frequency
x_scaled = rescale(x, 60, 84);  % MIDI range
frequencies = 440 * 2.^((x_scaled - 69)/12);  % Hz

% y as volume
amplitudes = rescale(abs(y), 0.1, 0.9);
t = linspace(0, note_duration, round(Fs * note_duration));

melody = [];

% Generate waveform per note
for i = 1:N
    f = frequencies(i);
    A = amplitudes(i);
    phase = 2 * pi * f * t;
    switch waveform
        case 'sine'
            note = A * sin(phase);
        case 'square'
            note = A * square(phase);
        case 'triangle'
            note = A * sawtooth(phase, 0.5);  % triangle is symmetric sawtooth
        case 'saw'
            note = A * sawtooth(phase);
    end
    melody = [melody, note];
end

% Play and save
sound(melody, Fs);
%audiowrite(['henon_' waveform '_melody.wav'], melody, Fs);

%frequency plot
figure;
plot(frequencies, '.-');
xlabel('Note Index'); ylabel('Frequency (Hz)');
title(['Hénon → Pitch Mapping using ' waveform ' wave']);
