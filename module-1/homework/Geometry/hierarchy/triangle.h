#include "polygon.h"
#include "circle.h"

class Triangle : public Polygon {
public:
    Circle circumscribedCircle();
    Circle inscribedCircle();
};