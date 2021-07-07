#include <utility>
#include <cmath>
#include "polygon.h"
#include "ellipse.h"
#include "Constants.h"

Polygon::Polygon(std::vector<Point> points_) : points(std::move(points_)) {}

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
    Point p1 = points_copy[0];
    Point p2 = points_copy[1];
    Point p3 = points_copy[2];
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
    return std::abs(p_area * 0.5);
}

size_t Polygon::find_shift(const Polygon& another_polygon) const {
    Point start_point = another_polygon.getVertices()[0];
    size_t start_index = -1;
    for (size_t i = 0; i < points.size(); i++) {
        if (points[i] == start_point) {
            start_index = i;
            break;
        }
    }
    return start_index;
}

bool Polygon::operator==(const Shape& another) const {
    const auto* another_polygon = dynamic_cast<const Polygon*>(&another);
    if (another_polygon == nullptr) {
        return false;
    }
    if (another_polygon->verticesCount() != verticesCount()) {
        return false;
    }
    size_t index = find_shift(*another_polygon);
    if (index == -1) {
        return false;
    }
    for (size_t i = 0; i < points.size(); i++) {
        if (another_polygon->getVertices()[i] != points[(index++) % points.size()]) {
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
    size_t index = find_shift(*another_polygon);
    if (index == -1) {
        return false;
    }
    for (size_t i = 0; i < points.size(); i++) {
        double this_dist = Point::dist(points[index], points[(index + 1) % points.size()]);
        double another_dist = Point::dist(another_polygon->getVertices()[i],
                another_polygon->getVertices()[(i + 1) % points.size()]);
        if (this_dist != another_dist) {
            return false;
        }
        index = (index + 1) % points.size();
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

    double angle_sum = 0;
    for (size_t i = 0; i < points.size(); i++) {
        Vector p1P = Vector(points[i], point);
        Vector p2P = Vector(points[(i + 1) % points.size()], point);
        if ((p1P^p2P) == 0 || p1P*p2P <= 0) {
            return true;
        } else {
            angle_sum += std::atan2((p1P ^ p2P), p1P * p2P);
        }
    }
    return angle_sum - 2 * Constants::PI == 0;
}

void Polygon::rotate(const Point& center, double angle) {
    double rad_angle = angle * Constants::PI / 180;
    std::vector<Point> new_points;
    for (auto point : points) {
        new_points.push_back(Shape::rotatePoint(point, center, rad_angle));
    }
    points = new_points;
}

void Polygon::reflex(const Point& center) {
    std::vector<Point> new_points;
    for (auto point : points) {
        new_points.push_back(Shape::reflectPoint(point, center));
    }
    points = new_points;
}

void Polygon::reflex(const Line& axis) {
    std::vector<Point> new_points;
    for (auto point : points) {
        new_points.push_back(Shape::reflectPoint(point, axis));
    }
    points = new_points;
}

void Polygon::scale(const Point& center, double scale) {
    std::vector<Point> new_points;
    for (auto point : points) {
        new_points.push_back(Shape::scalePoint(point, center, scale));
    }
    points = new_points;
}

