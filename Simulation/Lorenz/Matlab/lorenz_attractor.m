function X = lorenz_attractor(r,s,b,x0,y0,z0,T_max,dt)
    % Funzione che calcola il moto di un attrattore di Lorenz a partire
    % dalle equazioni differenziali e stampa un plot.
    % INPUT:
    % r,s,b - parametri del sistema
    % [x0,y0,z0] - condizioni iniziali
    % T_max - tempo massimo di esecuzione
    % dt - passo temporale
    %
    % OUTPUT
    % X - matrice Nx3 in cui è memorizzata la traiettoria
    
    % Calcolo traiettoria con metodo RK a 4 stadi
    lorenz = @(X) [s*(X(2)-X(1)), r*X(1) - X(2) - X(1)*X(3), X(1)*X(2) - b*X(3)];
    N = T_max/dt;
    X0 = [x0,y0,z0];
    X = zeros(N,3);
    X(1,:) = X0;
    for i = 1:N-1
        K1 = lorenz(X(i,:));
        K2 = lorenz(X(i,:)+K1*dt/2);
        K3 = lorenz(X(i,:)+K2*dt/2);
        K4 = lorenz(X(i,:)+K3*dt);
        X(i+1,:) = X(i,:)+dt*(K1+2*K2+2*K3+K4)/6;
    end

    %tspan = 0:dt:T_max;
    %[~,X] = ode45(lorenz, tspan, X0);
    
    % Plot della traiettoria
    figure; clf;
    plot3(X(:,1), X(:,2), X(:,3), 'b', 'LineWidth', 1);
    hold off;
    hold on;
    plot3(X(1,1), X(1,2), X(1,3), 'go', 'MarkerSize', 3, 'MarkerFaceColor', 'g'); % Punto iniziale
    plot3(X(end,1), X(end,2), X(end,3), 'ro', 'MarkerSize', 3, 'MarkerFaceColor', 'r'); % Punto finale
    grid on;
    axis equal;
    legend({'Traiettoria', 'Inizio', 'Fine'}, 'Location', 'Best');
    title("Moto dell'attrattore di Lorenz", ['rho = ', num2str(r), '; ', 'sigma = ', num2str(s), '; ', 'beta = ', num2str(b),'; ']);
end