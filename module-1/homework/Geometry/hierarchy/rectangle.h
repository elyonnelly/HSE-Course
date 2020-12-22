#include <utility>
#include "polygon.h"

class Rectangle : public Polygon {
public:
    Rectangle(const Point& p1, const Point& p2, double ratio);
    Point center();
    std::pair<Line, Line> diagonals();
};