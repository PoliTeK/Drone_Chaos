

u = 0.9;
k = 0.4;
p = 6.0;


N = 50000;

x = zeros(1, N);
y = zeros(1, N);


x(1) = 0.1;
y(1) = 0.1;


for n = 1:N-1
    r2 = x(n)^2 + y(n)^2;
    theta = k - p / (1 + r2);
    x(n+1) = 1 + u * (x(n)*cos(theta) - y(n)*sin(theta));
    y(n+1) = u * (x(n)*sin(theta) + y(n)*cos(theta));
end


figure;
plot(x, y, '.', 'MarkerSize', 0.5);
title('Ikeda Attractor');
xlabel('x'); ylabel('y');
axis equal;
grid on;
