
#include "square.h"
#include <cmath>

//описанная окружность
Circle Square::circumscribedCircle() {
    return Circle(center(), Point::dist(points[0], points[2]) / (2 * std::sqrt(2)));
}

//вписанная окружность
Circle Square::inscribedCircle() {
    return Circle(center(), Point::dist(points[0], points[1]) / 2);
}
