
#include "circle.h"

double Circle::radius() {
    return dist;
}

Circle::Circle(const Point& center_, double mRadius) : Ellipse(center_, center_, mRadius) { }
