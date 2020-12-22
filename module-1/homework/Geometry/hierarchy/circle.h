#ifndef GEOMETRY_ELLIPSE_H
#define GEOMETRY_ELLIPSE_H

#include "ellipse.h"

class Circle : public Ellipse {
public:
    Circle(const Point& center_, double mRadius);
    double radius();
};

#endif //GEOMETRY_ELLIPSE_H