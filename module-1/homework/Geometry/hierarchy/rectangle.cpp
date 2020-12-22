#include "rectangle.h"
#include "ellipse.h"
#include <cmath>

Rectangle::Rectangle(const Point& p1, const Point& p2, double ratio) : Polygon() {
    //p1 как бы B, p2 как бы D
    Point B = p1, D = p2;
    Point center = Shape::centerOfSegment(B, D);
    double diagonal_length = Point::dist(B, D);
    double BC = sqrt(diagonal_length * diagonal_length / (ratio * ratio + 1));
    double AB = BC * ratio;
    double BO = Point::dist(p1, center);
    double alpha = std::acos(1 - (std::pow(AB, 2) / (2 * std::pow(BO, 2))));
    Point A = Shape::rotatePoint(B, center, alpha);
    Point C = Shape::rotatePoint(B, center, -(Shape::PI - alpha));
    points = {A, B, C, D};
    place_points_clockwise();
}

Point Rectangle::center() {
    return Shape::centerOfSegment(points[0], points[2]);
}

std::pair<Line, Line> Rectangle::diagonals() {
    return {Line(points[0], points[2]), Line(points[1], points[3])};
}
