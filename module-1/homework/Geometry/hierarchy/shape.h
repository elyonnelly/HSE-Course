#ifndef GEOMETRY_SHAPE_H_
#define GEOMETRY_SHAPE_H_

#include "line.h"

class Shape {
public:
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
    virtual bool operator==(const Shape& another) const = 0;
    virtual bool isCongruentTo(const Shape& another) const = 0;
    virtual bool isSimilarTo(const Shape& another) const = 0;
    virtual bool containsPoint(const Point& point)const = 0;
    virtual void rotate(const Point& center, double angle) = 0;
    virtual void reflex(const Point& center) = 0;
    virtual void reflex(const Line& axis) = 0;
    virtual void scale(const Point& center, double coefficient) = 0;

    static Point reflectPoint(const Point& point, const Line& axis);
    static Point reflectPoint(const Point& point, const Point& center);
    static Point rotatePoint(const Point& point, const Point& center, double angle);
    static Point scalePoint(const Point& point, const Point& center, double scale);
    static Point centerOfSegment(const Point& p1, const Point& p2);

    constexpr static const double ROUND_POW = 1;
    constexpr static const double EPS = 1000000;
    constexpr static const double PI = 3.1415926;
};

#endif