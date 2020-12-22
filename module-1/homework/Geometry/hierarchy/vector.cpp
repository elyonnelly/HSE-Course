#include "vector.h"
#include "shape.h"
#include <cmath>

double operator^(const Vector& v1, const Vector& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

double operator*(const Vector& v1, const Vector& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

Vector::Vector(double x_, double y_) : x(x_), y(y_) {}


Vector::Vector(const Point& point) : x(point.x), y(point.y) {}


Vector::Vector(const Point& A, const Point& B) {
    this->x = B.x - A.x;
    this->y = B.y - A.y;
}

Vector operator*(const Vector& vector, double scale) {
    return {vector.x * scale, vector.y * scale};
}

Point operator+(const Point& point, const Vector& vector) {
    return {point.x + vector.x, point.y + vector.y};
}

Point operator-(const Point& point, const Vector& vector) {
    return {point.x - vector.x, point.y - vector.y};
}


Vector Vector::getNormal() const {
    return {-y, x};
}

double Vector::getLength() const {
    return sqrt(x * x + y * y);
}

void Vector::normalize() {
    Vector n = *this * (1 / getLength());
    x = n.x;
    y = n.y;
}

Vector Vector::getRotatedVector(double angle) {
    return {std::round((x * std::cos(angle) - y * std::sin(angle)) * Shape::ROUND_POW) / Shape::ROUND_POW,
            std::round((y * std::cos(angle) + x * std::sin(angle)) * Shape::ROUND_POW) / Shape::ROUND_POW};
}
