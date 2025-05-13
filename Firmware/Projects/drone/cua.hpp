#include <vector>
#include <cmath>
#include <functional>

class Chua {
private:
    std::vector<double> X_calc = {0.1, 0.0, 0.0}; // Stato iniziale: x, y, z

    const double _alpha = 18.39;
    const double _beta = 39.0;
    const double _m0 = -1.143;
    const double _m1 = -0.714;

    double chua_diode(double x) const {
        return _m1 * x + 0.5 * (_m0 - _m1) * (fabs(x + 1) - fabs(x - 1));
    }

    // Sistema ODE di Chua
    std::vector<double> chua_ode(const std::vector<double>& X) const {
        double x = X[0], y = X[1], z = X[2];

        double dxdt = _alpha * (y - x - chua_diode(x));
        double dydt = x - y + z;
        double dzdt = -_beta * y;

        return {dxdt, dydt, dzdt};
    }

    // Metodo Runge-Kutta 4 per un passo
    void rungeKutta4(double dt) {
        std::vector<double> k1, k2, k3, k4, temp(X_calc.size());

        k1 = chua_ode(X_calc);

        for (size_t i = 0; i < X_calc.size(); i++)
            temp[i] = X_calc[i] + dt * k1[i] / 2.0;
        k2 = chua_ode(temp);

        for (size_t i = 0; i < X_calc.size(); i++)
            temp[i] = X_calc[i] + dt * k2[i] / 2.0;
        k3 = chua_ode(temp);

        for (size_t i = 0; i < X_calc.size(); i++)
            temp[i] = X_calc[i] + dt * k3[i];
        k4 = chua_ode(temp);

        for (size_t i = 0; i < X_calc.size(); i++)
            X_calc[i] += dt * (k1[i] + 2.0*k2[i] + 2.0*k3[i] + k4[i]) / 6.0;
    }

public:
    // Esegue un passo temporale di lunghezza dt e restituisce il nuovo stato
    void step(double dt) {
        rungeKutta4(dt);
    }

    void reset(const std::vector<double>& initialState) {
        if (initialState.size() == 3)
            X_calc = initialState;
    }

    // Per ottenere lo stato attuale
    std::vector<double> getState() const {
        return X_calc;
    }
};
