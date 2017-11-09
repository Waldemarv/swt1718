#include "point.h"

Point::Point() {
    this->x = 0;
    this->y = 0;
}

Point::~Point() {
}

Point::Point(double nx, double ny) {
    this->x=nx;
    this->y=ny;
}

void Point::setX(double nx) {
    this->x=nx;
}
void Point::setY(double ny) {
    this->y=ny;
}

double Point::getX() {
    return this->x;
}

double Point::getY() {
    return this->y;
}

Point Point::operator = (Point* np) {
    Point* point = new Point(np->x,np->y);
    return *point;
}
