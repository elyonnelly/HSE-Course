#ifndef GEOMETRY_POLYGON_H_
#define GEOMETRY_POLYGON_H_

#include "shape.h"
#include <vector>

class Polygon : public Shape {
public:
    Polygon() = default;
    explicit Polygon(std::vector<Point> points_);
    int verticesCount() const ;
    std::vector<Point> getVertices() const;
    bool isConvex() const;

    double perimeter() const override;
    double area() const override;
    bool operator==(const Shape& another) const override;
    bool isCongruentTo(const Shape& another) const override;
    bool isSimilarTo(const Shape& another) const override;
    bool containsPoint(const Point& point) const override;
    void rotate(const Point& center, double angle) override;
    void reflex(const Point& center) override;
    void reflex(const Line& axis) override;
    void scale(const Point& center, double scale) override;

protected:
    std::vector<Point> points;

private:
    size_t find_shift(const Polygon& another_polygon) const;
};

#endif