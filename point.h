#ifndef POINT_H
#define POINT_H
/*! Definiert einen Punkt in der x-y-Ebene */
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
