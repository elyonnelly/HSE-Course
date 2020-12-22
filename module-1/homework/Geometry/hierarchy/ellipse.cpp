
#include "ellipse.h"
#include "Constants.h"
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
    return Shape::centerOfSegment(focus1, focus2);
}

double Ellipse::perimeter() const {
    double semi_major_axis = getSemiMajorAxis();
    double semi_minor_axis = getSemiMinorAxis();

    return 4 * (Constants::PI * semi_minor_axis * semi_major_axis + (semi_major_axis - semi_minor_axis))
                / (semi_major_axis + semi_minor_axis);
}

double Ellipse::area() const {
    return Constants::PI * getSemiMinorAxis() * getSemiMajorAxis();
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
    focus1 = Shape::rotatePoint(focus1, center, angle);
    focus2 = Shape::rotatePoint(focus2, center, angle);
}

void Ellipse::reflex(const Point& center) {
    focus1 = Shape::reflectPoint(focus1, center);
    focus2 = Shape::reflectPoint(focus2, center);
}

void Ellipse::reflex(const Line& axis) {
    focus1 = Shape::reflectPoint(focus1, axis);
    focus2 = Shape::reflectPoint(focus2, axis);
}

void Ellipse::scale(const Point& center, double scale) {
    Point point_on_ellipse = this->center() + Vector(0, getSemiMajorAxis());
    Point scaled_point = Shape::scalePoint(point_on_ellipse, center, scale);
    focus1 = Shape::scalePoint(focus1, center, scale);
    focus2 = Shape::scalePoint(focus2, center, scale);
    dist = Point::dist(focus1, scaled_point) + Point::dist(focus2, scaled_point);
}

double Ellipse::getSemiMajorAxis() const {
    return dist / 2;
}

double Ellipse::getSemiMinorAxis() const {
    double focal_distance = getSemiFocalDistance();
    double semi_major_axis = getSemiMajorAxis();
    return std::sqrt(std::pow(semi_major_axis, 2) - std::pow(focal_distance, 2));
}

double Ellipse::getSemiFocalDistance() const {
    return Point::dist(focus1, focus2) / 2;
}
