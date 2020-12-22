#include <utility>
#include<vector>
#include <algorithm>
#include "polygon.h"

Polygon::Polygon(std::vector<Point> points_) : points(std::move(points_)) {
    place_points_clockwise();
}

int Polygon::verticesCount() const {
    return points.size();
}

std::vector<Point> Polygon::getVertices() const {
    return points;
}

bool Polygon::isConvex() const {
    if (points.size() < 3) {
        return false;
    }
    std::vector<Point> points_copy = points;
    Point p1 = points_copy[0], p2 = points_copy[1], p3 = points_copy[2];
    points_copy.push_back(points_copy[0]);
    Vector p1p2(p1, p2), p1p3(p1, p3);
    bool product_sign = (p1p2 ^ p1p3) > 0;
    for (size_t i = 3; i < points_copy.size(); i++) {
        p1 = p2;
        p2 = p3;
        p3 = points_copy[i];
        p1p2 = Vector(p1, p2), p1p3 = Vector(p1, p3);
        double product = p1p2 ^p1p3;
        if ((product > 0) != product_sign) {
            return false;
        }
    }
    return true;
}

double Polygon::perimeter() const {
    double P = 0;
    for (size_t i = 0; i < points.size(); i++) {
        P += Point::dist(points[i], points[(i + 1) % points.size()]);
    }
    return P;
}

double Polygon::area() const {
    //Gauss formula
    double p_area = 0;
    for (size_t i = 0; i < points.size() - 1; i++) {
        p_area += points[i].x * points[i + 1].y;
    }
    p_area += points[points.size() - 1].x * points[0].y;
    for (size_t i = 0; i < points.size() - 1; i++) {
        p_area -= points[i + 1].x * points[i].y;
    }
    p_area -= points[0].x * points[points.size() - 1].y;
    return p_area * 0.5;
}

bool Polygon::operator==(const Shape& another) const {
    const auto* another_polygon = dynamic_cast<const Polygon*>(&another);
    if (another_polygon == nullptr) {
        return false;
    }
    if (another_polygon->verticesCount() != verticesCount()) {
        return false;
    }
    for (size_t i = 0; i < points.size(); i++) {
        if (another_polygon->getVertices()[i] != points[i]) {
            return false;
        }
    }
    return true;
}

bool Polygon::isCongruentTo(const Shape& another) const {
    const auto* another_polygon = dynamic_cast<const Polygon*>(&another);
    if (another_polygon == nullptr) {
        return false;
    }
    if (another_polygon->verticesCount() != verticesCount()) {
        return false;
    }
    for (size_t i = 0; i < points.size(); i++) {
        double this_dist = Point::dist(points[i], points[(i + 1) % points.size()]);
        double another_dist = Point::dist(another_polygon->getVertices()[i],
                another_polygon->getVertices()[(i + 1) % points.size()]);
        if (this_dist != another_dist) {
            return false;
        }
    }
    return true;
}

bool Polygon::isSimilarTo(const Shape& another) const {
    const auto* another_polygon = dynamic_cast<const Polygon*>(&another);
    if (another_polygon == nullptr) {
        return false;
    }
    if (verticesCount() != another_polygon->verticesCount()) {
        return false;
    }
    if (verticesCount() == 1) {
        return true;
    }

    double aspect_ratio = Point::dist(points[0], points[1]) /
                          Point::dist(another_polygon->getVertices()[0], another_polygon->getVertices()[1]);
    for (size_t i = 1; i < points.size(); i++) {
        double next_aspect_ratio = Point::dist(points[i - 1], points[i]) /
                                   Point::dist(another_polygon->getVertices()[i - 1],
                                               another_polygon->getVertices()[i]);
        if (aspect_ratio != next_aspect_ratio) {
            return false;
        }
    }
    return true;
}

bool Polygon::containsPoint(const Point& point) const {
    if (points.size() < 3) {
        return false;
    }
    std::vector<Point> points_copy = points;
    points_copy.push_back(points_copy[0]);
    Point p1 = points_copy[0], p2 = points_copy[1];
    Vector p1p2(p1, p2), p1P(p1, point);
    bool product_sign = (p1p2 ^ p1P) > 0;
    for (size_t i = 2; i < points_copy.size(); i++) {
        p1 = p2;
        p2 = points_copy[i];
        p1p2 = Vector(p1, p2), p1P = Vector(p1, point);
        double product = p1p2 ^p1P;
        if ((product > 0) != product_sign) {
            return false;
        }
    }
    return true;
}

void Polygon::rotate(const Point& center, double angle) {

}

void Polygon::reflex(const Point& center) {
    std::vector<Point> new_points;
    for (auto point : points) {
        Vector transfer_vector(point, center);
        transfer_vector = transfer_vector * 2;
        Point new_point = point + transfer_vector;
        new_points.push_back(new_point);
    }
    points = new_points;
    place_points_clockwise();
}

void Polygon::reflex(const Line& axis) {
    std::vector<Point> new_points;
    for (auto point : points) {
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
        new_points.push_back(new_point);
    }
    points = new_points;
    place_points_clockwise();
}

void Polygon::scale(const Point& center, double scale) {

}

void Polygon::place_points_clockwise() {
    std::sort(points.begin(), points.end());
    Point start = points[0];
    //sort by polar angle, placing points clockwise
    std::sort(++points.begin(), points.end(), Comparator(start));
}

