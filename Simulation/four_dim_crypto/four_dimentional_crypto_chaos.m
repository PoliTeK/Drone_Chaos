N = 5000;  % number of iterations
X = zeros(4, N);
Y = zeros(4, N);

% Initial conditions
X(:,1) = [0.2; 0.2; 0.2; 0.2];
A = 5 * [-7  8  7 -3;
         -3  1 -2  3;
          2 10 -3 -4;
         -4 -6  1 -9];

Y(:,1) = A * X(:,1) + [0.8198; 0.4405; -0.2705; -0.7534];

% Iteration loop
for k = 1:N-1
    % Driving system X
    x1 = X(1,k);
    x2 = X(2,k);
    x3 = X(3,k);
    x4 = X(4,k);
    
    X(1,k+1) = 1.3 * cos(x2) + sin(x3);
    X(2,k+1) = 2 * x1 * sin(x2) - x1 * cos(x2);
    X(3,k+1) = 1.2 * x1 + sin(x3);
    X(4,k+1) = sin(x1 * x3 + x2 + x4);
    
    % q correction term
    q = (A * X(:,k) - Y(:,k)) / 12;

    % Driven system Y
    Fx = [X(1,k+1); X(2,k+1); X(3,k+1); X(4,k+1)];
    Y(:,k+1) = A * Fx - q;
end

%synchronization error
%errors = vecnorm(A * X - Y);
%figure;
%plot(1:N, errors);
%title('Synchronization Error ||AX - Y||');
%xlabel('Iteration');
%ylabel('Error Norm');

%phase plot

figure;
plot3(X(1,:), X(2,:), X(3,:));
title('Chaotic Attractor of X');
xlabel('x1'); ylabel('x2'); zlabel('x3');
