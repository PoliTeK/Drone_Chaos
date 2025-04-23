a = 1.41;
b = 0.3;
N = 10000;

x = zeros(1, N);
y = zeros(1, N);
x(1) = 0.1;
y(1) = 0.3;

for n = 2:N
    x(n) = 1 - a*x(n-1)^2 + y(n-1);
    y(n) = b*x(n-1);
end

figure;
plot(x, y, '.k', 'MarkerSize', 1);
title('Hénon Attractor');
xlabel('x'); ylabel('y');
axis equal;