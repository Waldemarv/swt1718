#include "point.h"

Point::Point() {
    this->x = 0;
    this->y = 0;
}

Point::~Point() {
}

Point::Point(double nx, double ny) {
    x=nx;
    y=ny;
}

void Point::setX(double nx) {
    x=nx;
}
void Point::setY(double ny) {
    y=ny;
}

double Point::getX() {
    return x;
}

double Point::getY() {
    return y;
}

Point Point::operator = (Point* np) {
    Point* point = new Point(np->x,np->y);
    return *point;
}
