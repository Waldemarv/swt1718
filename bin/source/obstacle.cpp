#include "obstacle.h"

void Obstacle::drawObstacle() {
}

QString Obstacle::getType()
{
    return "staticObstacle";
}

double Obstacle::getlength() { return length; }
double Obstacle::getwidth() {return width; }

Point* Obstacle::getPosition() { return position; }

Obstacle::Obstacle(){
    position = new Point();
}
Obstacle::Obstacle(double x, double y, double width, double length) : width(width), length(length)
{
    position = new Point(x,y);
}
