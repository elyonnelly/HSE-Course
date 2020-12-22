#include "point.h"
#include "shape.h"
#include <cmath>

bool operator==(const Point& p1, const Point& p2) {
    return (std::abs(p1.x - p2.x) < Shape::EPS) && (std::abs(p1.y - p2.y) < Shape::EPS);
}

bool operator!=(const Point& p1, const Point& p2) {
    return !(p1 == p2);
}

Point::Point(double x_, double y_) : x(x_), y(y_) {}

bool operator<(const Point& p1, const Point& p2) {
    if (p1.x != p2.x) {
        return p1.x < p2.x;
    }
    return p1.y < p2.y;
}

double Point::dist(const Point& a, const Point& b) {
    return std::round(sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y-b.y)) * Shape::ROUND_POW) / Shape::ROUND_POW;
}

Point Point::roundCoordinates() {
    x = std::round(x * Shape::ROUND_POW) / Shape::ROUND_POW;
    y = std::round(y * Shape::ROUND_POW) / Shape::ROUND_POW;
    return *this;
}
