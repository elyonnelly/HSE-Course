struct Point {
    Point() : Point(0, 0){}
    Point(double x_, double y_);
    double x;
    double y;

    friend bool operator==(const Point &p1, const Point &p2);
    friend bool operator!=(const Point &p1, const Point &p2);
    friend bool operator<(const Point &p1, const Point &p2);

    static double dist(const Point& p1, const Point& p2);
};