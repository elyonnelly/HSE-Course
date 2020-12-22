#include "point.h"

struct Vector {
    Vector(double x_, double y_);
    Vector(const Point& point);
    Vector(const Point& A, const Point& B);
    double x;
    double y;

    Vector getNormal() const;
    double getLength() const;
    void normalize();

    friend Vector operator*(const Vector& vector, double scale);
    friend Point operator+(const Point& point, const Vector& vector);
    friend Point operator-(const Point& point, const Vector& vector);
    //pseudoscalar product
    friend double operator^(const Vector& v1, const Vector& v2);

    Vector getRotatedVector(double angle);
};