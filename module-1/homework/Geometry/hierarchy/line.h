#ifndef GEOMETRY_LINE_H_
#define GEOMETRY_LINE_H_

#include "vector.h"

class Line {
public:
    Line(Point p1_, Point p2_);

    friend bool operator==(const Line& a, const Line& b);
    friend bool operator!=(const Line& a, const Line& b);

    Vector getDirectionVector() const;
    Point getP1() const;
    Point getP2() const;

private:
    Point p1;
    Point p2;
};

#endif //GEOMETRY_LINE_H