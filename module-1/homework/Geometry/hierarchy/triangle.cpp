
#include "triangle.h"
#include <cmath>

Circle Triangle::circumscribedCircle() {
    Point A = points[0];
    Point B = points[1];
    Point C = points[2];
    double d = 2 * (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));
    double x = ((std::pow(A.x, 2) + std::pow(A.y, 2)) * (B.y - C.y) +
                (std::pow(B.x, 2) + std::pow(B.y, 2)) * (C.y - A.y) +
                (std::pow(C.x, 2) + std::pow(C.y, 2)) * (A.y - B.y)) / d;
    double y = ((std::pow(A.x, 2) + std::pow(A.y, 2)) * (C.x - B.x) +
                (std::pow(B.x, 2) + std::pow(B.y, 2)) * (A.x - C.x) +
                (std::pow(C.x, 2) + std::pow(C.y, 2)) * (B.x - A.x)) / d;
    Point center = Point(x, y);
    return Circle(center, Point::dist(A, center));
}

Circle Triangle::inscribedCircle() {
    Point A = points[0];
    Point B = points[1];
    Point C = points[2];
    double AB = Point::dist(A, B);
    double BC = Point::dist(B, C);
    double AC = Point::dist(C, A);
    double x = (BC * A.x + AC * B.x + AB * C.x) / (BC + AC + AB);
    double y = (BC * A.y + AC * B.y + AB * C.y) / (BC + AC + AB);
    double radius = area() / (perimeter() / 2);
    Point center = Point(x, y);
    return Circle(center, radius);
}
