#include <cmath>
#include <functional>


class Henon
{
private:
    double _x0;
    double _y0;
    double _a;
    double _b;
public:
    Henon(/* args */);
    ~Henon();
    void henonIncrement() {

    _x0 = 1 - _a*_x0*_x0 + _y0;
    _y0 = _b*_x0;
}
    double getX() {
        return _x0;
    }
    double getY() {
        return _y0;
    }
    void setX(double x) {
        _x0 = x;
    }
    void setY(double y) {
        _y0 = y;
    }
};

Henon::Henon(/* args */)
{
    _x0 = 0.1;
    _y0 = 0.3;
    _a = 1.41;
    _b = 0.3;
}

Henon::~Henon()
{
}


