#include "line.h"

bool operator==(const Line& a, const Line& b) {
    Vector p1(a.p1, a.p2);
    Vector p2(b.p1, b.p2);
    Vector m(a.p1, b.p1);
    return (p1^p2) == 0 && (p1^m) == 0 && (p2^m) == 0;
}

bool operator!=(const Line& a, const Line& b) {
    return !(a == b);
}

Line::Line(Point p1_, Point p2_) : p1(p1_), p2(p2_) {}

Vector Line::getDirectionVector() const {
    return {p1, p2};
}

Point Line::getP1() const {
    return p1;
}

Point Line::getP2() const {
    return p2;
}
