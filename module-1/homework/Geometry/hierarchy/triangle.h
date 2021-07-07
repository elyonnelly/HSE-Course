#ifndef GEOMETRY_TRIANGLE_H_
#define GEOMETRY_TRIANGLE_H_

#include "polygon.h"
#include "circle.h"

class Triangle : public Polygon {
public:
    Triangle(const Point& a, const Point& b, const Point& c) : Polygon({a, b, c}) {}
    Circle circumscribedCircle();
    Circle inscribedCircle();
};

#endif