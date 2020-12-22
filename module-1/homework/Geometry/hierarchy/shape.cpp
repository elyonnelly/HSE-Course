
#include "shape.h"
#include <utility>
#include <cmath>

Point Shape::rotatePoint(const Point& point, const Point& center, double angle) {
    Vector CP = Vector(center, point);
    Vector rotated_CP = CP.getRotatedVector(angle);
    return point + rotated_CP;
}

Point Shape::reflectPoint(const Point& point, const Line& axis) {
    Vector direction_vector = axis.getDirectionVector();
    Vector perpendicular = direction_vector.getNormal();
    Vector AP = Vector(axis.getP1(), point);
    Vector AB = Vector(axis.getP1(), axis.getP2());
    double h = std::abs(AP ^ AB) / AB.getLength();
    perpendicular.normalize();
    perpendicular = perpendicular * h;
    Point new_point;
    if ((AP ^ AB) > 0) {
        new_point = point + perpendicular * 2;
    } else {
        new_point = point - perpendicular * 2;
    }
    return new_point;
}

Point Shape::reflectPoint(const Point& point, const Point& center) {
    Vector transfer_vector(point, center);
    transfer_vector = transfer_vector * 2;
    return point + transfer_vector;
}
