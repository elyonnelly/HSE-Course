#include "rectangle.h"
#include <cmath>

Rectangle::Rectangle(const Point& p1, const Point& p2, double ratio) {
    Point center = Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
    double diagonal_length = Point::dist(p1, p2);
    //известно что a^2 + b^2 = diagonal_length^2
    //также известно, если а это меньшая сторона, то a = b * ratio (<= ratio = a/b)
    //тогда (b*ratio)^2 + b^2 = diag_lenth^2
    //b^2*ratio^2+b^2=diag^2
    //b^2(ratio^2+1)=diag^2
    //b^2=diag^2/(ratio^2+1)
    //B _____ C
    // |  .  |
    //A|_____|D
    double BC = sqrt(diagonal_length * diagonal_length / (ratio * ratio + 1));
    double AB = BC * ratio;
    //кажется будто не так все же вычисление проходит.
    // потому что может быть слегка повернут прямоугольник.
    Point A = Point(center.x - BC / 2, center.y - AB / 2);
    Point B = Point(center.x - BC / 2, center.y + AB / 2);
    Point C = Point(center.x + BC / 2, center.y + AB / 2);
    Point D = Point(center.x + BC / 2, center.y - AB / 2);
    points = {A, B, C, D};
}

Point Rectangle::center() {
    return Point((points[0].x + points[2].x) / 2, (points[0].y + points[2].y) / 2);
}

std::pair<Line, Line> Rectangle::diagonals() {
    return {Line(points[0], points[2]), Line(points[1], points[3])};
}
