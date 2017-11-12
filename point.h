#ifndef POINT_H
#define POINT_H

class Point
{
private:
    double x;
    double y;
public:
    Point();
    Point(double nx, double ny);
    ~Point();
    void setX(double nx);
    void setY(double ny);
    double getX();
    double getY();
    Point operator = (Point* np);
};

#endif // POINT_H
