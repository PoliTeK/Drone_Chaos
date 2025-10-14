function out = thomas_attractor_()
%     x' = sin(y) - b*x
%     y' = sin(z) - b*y
%     z' = sin(x) - b*z
%
%   Returns struct with time, states, and basic diagnostics.

% User parameters
b        = 0.23;          % dissipation / chaos parameter (try 0.18–0.33)
x0       = [0.1; -0.1; 0.2];% initial condition
tmax     = 400;             % total integration time, let it go for longer and the curve will change of course
t_drop   = 50;              % drop this initial transient when plotting/analyzing
rtol     = 1e-9;            % ODE tolerances (tight for clean curves)
atol     = 1e-12;

% Optional extras:
makePoincare = true;        % compute Poincaré section at z ≈ 0 (z increasing)
estimateLLE  = true;        % quick-and-dirty largest Lyapunov exponent estimate

%Integrate
f = @(t,X) [sin(X(2))-b*X(1);
            sin(X(3))-b*X(2);
            sin(X(1))-b*X(3)];

opts = odeset('RelTol',rtol,'AbsTol',atol);
[t, X] = ode45(f, [0 tmax], x0, opts);

% Remove transient
keep = t >= t_drop;
t = t(keep); X = X(keep,:);

%Plotting 
figure('Color','w'); 
tclr = (t - t(1)) / (t(end)-t(1));  % normalized time for color
% 3D trajectory colored by time
subplot(2,2,[1 3]); hold on; box on; grid on;
scatter3(X(:,1), X(:,2), X(:,3), 3, tclr, 'filled');
plot3(X(1,1), X(1,2), X(1,3), 'k.', 'MarkerSize', 18);
xlabel('x'); ylabel('y'); zlabel('z');
title(sprintf('Thomas attractor, b = %.5f', b));
view(36,24);
axis tight; daspect([1 1 1]); colormap turbo; colorbar; 

% 2D projections
subplot(2,2,2); plot(X(:,1), X(:,2), 'k-', 'LineWidth', 0.6);
axis equal tight; grid on; xlabel('x'); ylabel('y'); title('Projection (x,y)');
subplot(2,2,4); plot(X(:,1), X(:,3), 'k-', 'LineWidth', 0.6);
axis equal tight; grid on; xlabel('x'); ylabel('z'); title('Projection (x,z)');

%Poincaré section 
poincare = [];
if makePoincare
    % Section: z = 0 with dz/dt > 0
    z = X(:,3); dz = gradient(z, t);
    cross_idx = find(z(1:end-1) < 0 & z(2:end) >= 0 & dz(2:end) > 0);
    % Linear interpolation to refine crossing points
    for k = cross_idx'
        z1 = z(k); z2 = z(k+1);
        a = z1/(z1 - z2);              % fraction from k to k+1 at z=0
        xk = X(k,1) + a*(X(k+1,1)-X(k,1));
        yk = X(k,2) + a*(X(k+1,2)-X(k,2));
        poincare(end+1,:) = [xk, yk]; %#ok<AGROW>
    end

    if ~isempty(poincare)
        figure('Color','w'); 
        plot(poincare(:,1), poincare(:,2), '.', 'MarkerSize', 8);
        grid on; axis equal tight;
        xlabel('x (z=0, crossing up)'); ylabel('y (z=0, crossing up)');
        title(sprintf('Poincaré section (z=0, dz/dt>0), b = %.5f', b));
    end
end

%Largest Lyapunov exponent
lle = NaN;
if estimateLLE
    % Two-trajectory, periodic renormalization method (Benettin-style)
    dt     = mean(diff(t));
    delta0 = 1e-8;                     % initial separation
    Xref   = X(1,:).';
    Xpert  = Xref + delta0 * randn(3,1); Xpert = Xref + delta0*(Xpert - Xref)/norm(Xpert - Xref);

    Taccum = 0; ssum = 0; nsteps = 0;
    % Integrate along the *recorded* time vector for simplicity
    for k = 1:numel(t)-1
        tk = t(k); tk1 = t(k+1);
        [~, Yref]  = ode45(f, [tk tk1],  Xref,  opts);
        [~, Ypert] = ode45(f, [tk tk1], Xpert, opts);
        Xref  = Yref(end,:).';
        Xpert = Ypert(end,:).';
        d = norm(Xpert - Xref);
        if d == 0, d = eps; end
        ssum = ssum + log(d/delta0);
        nsteps = nsteps + 1;
        % Renormalize
        Xpert = Xref + delta0 * (Xpert - Xref) / d;
        Taccum = Taccum + (tk1 - tk);
    end
    lle = ssum / Taccum;  % 1/time
end

%Output
out = struct('b', b, 't', t, 'X', X, ...
             'poincare', poincare, ...
             'lle_estimate', lle, ...
             'settings', struct('tmax',tmax,'t_drop',t_drop,'x0',x0,...
                                'rtol',rtol,'atol',atol));

% Console summary
fprintf('Thomas attractor simulation done.\n');
fprintf('  b = %.5f, points kept = %d\n', b, size(X,1));
if ~isnan(lle)
    fprintf('  Largest Lyapunov exponent (quick estimate): %.5g  [1/time]\n', lle);
end

end
