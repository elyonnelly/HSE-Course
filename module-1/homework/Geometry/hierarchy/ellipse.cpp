
#include "ellipse.h"
#include <cmath>

Ellipse::Ellipse(const Point& f1, const Point& f2, double d) {
    if (f1 < f2) {
        focus1 = f1;
        focus2 = f2;
    } else {
        focus1 = f2;
        focus2 = f1;
    }
    dist = d;
}

std::pair <Line, Line> Ellipse::directrices() const {
    double k = eccentricity() / getSemiMajorAxis();
    Vector F1F2 = Vector(focus1, focus2);
    F1F2.normalize();
    Point center = this->center();
    Vector n = F1F2.getNormal();
    Point d11 = center + F1F2 * k;
    Point d12 = d11 + n;
    Point d21 = center - F1F2 * k;
    Point d22 = d21 + n;
    return {Line(d11, d12), Line(d21, d22)};
}

std::pair<Point, Point> Ellipse::focuses() const {
    return {focus1, focus2};
}

double Ellipse::eccentricity() const {
    return getSemiFocalDistance() / getSemiMajorAxis();
}

Point Ellipse::center() const {
    return {(focus1.x + focus2.x) / 2, (focus1.y + focus2.y) / 2};
}

double Ellipse::perimeter() const {
    double semi_major_axis = getSemiMajorAxis();
    double semi_minor_axis = getSemiMinorAxis();

    return 4 * (Ellipse::PI * semi_minor_axis * semi_major_axis + (semi_major_axis - semi_minor_axis))
                / (semi_major_axis + semi_minor_axis);
}

double Ellipse::area() const {
    return Ellipse::PI * getSemiMinorAxis() * getSemiMajorAxis();
}

bool Ellipse::operator==(const Shape& another) const {
    const auto* another_ellipse = dynamic_cast<const Ellipse*>(&another);
    if (another_ellipse == nullptr) {
        return false;
    }
    return focus1 == another_ellipse->focus1 &&
            focus2 == another_ellipse->focus2 &&
            dist == another_ellipse->dist;
}

bool Ellipse::isCongruentTo(const Shape& another) const {
    const auto* another_ellipse = dynamic_cast<const Ellipse*>(&another);
    if (another_ellipse == nullptr) {
        return false;
    }
    return getSemiFocalDistance() == another_ellipse->getSemiFocalDistance() &&
            getSemiMajorAxis() == another_ellipse->getSemiMajorAxis() &&
            dist == another_ellipse->dist;
}

bool Ellipse::isSimilarTo(const Shape& another) const {
    const auto* another_ellipse = dynamic_cast<const Ellipse*>(&another);
    if (another_ellipse == nullptr) {
        return false;
    }
    return getSemiMajorAxis() / another_ellipse->getSemiMajorAxis() == getSemiMinorAxis() / another_ellipse->getSemiMajorAxis();
}

bool Ellipse::containsPoint(const Point& point) const {
    Point center = this->center();
    Point shifted_point = point - Vector(center);
    return Point::dist(shifted_point, focus1) + Point::dist(shifted_point, focus2) <= dist;
}

void Ellipse::rotate(const Point& center, double angle) {
    Vector CF1 = Vector(center, focus1), CF2 = Vector(center, focus2);
    Vector rotated_CF1 = CF1.getRotatedVector(angle);
    Vector rotated_CF2 = CF2.getRotatedVector(angle);
    focus1 = focus1 + rotated_CF1;
    focus2 = focus2 + rotated_CF2;
}

void Ellipse::reflex(const Point& center) {

}

void Ellipse::reflex(const Line& axis) {

}

void Ellipse::scale(const Point& center, double scale) {

}

double Ellipse::getSemiMajorAxis() const {
    return dist / 2;
}

double Ellipse::getSemiMinorAxis() const {
    double focal_distance = getSemiFocalDistance();
    double semi_major_axis = getSemiMajorAxis();
    return std::sqrt(std::pow(semi_major_axis, 2) - std::sqrt(std::pow(focal_distance, 2)));
}

double Ellipse::getSemiFocalDistance() const {
    return Point::dist(focus1, focus2) / 2;
}
