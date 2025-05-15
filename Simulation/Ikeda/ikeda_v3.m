%parametri
u = 0.9;
k = 0.4;
p = 6.0;
N = 50000;

x = zeros(1, N);
y = zeros(1, N);
x(1) = 0.1;
y(1) = 0.1;


figure;
axis([-2 2 -2 2]);
xlabel('x'); ylabel('y');
title('Ikeda Attractor - Time Evolution with Color Gradient');
grid on;
hold on;


cmap = jet(N);  

for n = 1:N-1
    r2 = x(n)^2 + y(n)^2;
    theta = k - p / (1 + r2);
    x(n+1) = 1 + u * (x(n)*cos(theta) - y(n)*sin(theta));
    y(n+1) = u * (x(n)*sin(theta) + y(n)*cos(theta));

    
    if mod(n, 100) == 0
   
        color = cmap(n, :);
        
        
        trail_start = max(1, n - 99);
        scatter(x(trail_start:n), y(trail_start:n), 4, repmat(color, n-trail_start+1, 1), 'filled');
        
        drawnow;
    end
end
