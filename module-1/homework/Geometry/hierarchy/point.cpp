#include "point.h"
#include <cmath>

bool operator==(const Point& p1, const Point& p2) {
    return p1.x == p2.x && p1.y == p2.y;
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
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y-b.y));;
}