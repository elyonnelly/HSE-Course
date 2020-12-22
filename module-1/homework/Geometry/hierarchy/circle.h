#include "ellipse.h"

class Circle : public Ellipse {
public:
    Circle(const Point& center_, double mRadius);

    double radius();

private:
    Point center;
    double radius_;
};