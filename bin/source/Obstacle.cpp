#include "Obstacle.h"

void Obstacle::drawObstacle() {
}

double Obstacle::getlength() { return length; }
double Obstacle::getwidth() {return width; }

Point Obstacle::getPosition() { return position; }

Obstacle::Obstacle(){}
Obstacle::Obstacle(double x, double y, double width, double length) : width(width), length(length)
{
    position.setX(x);
    position.setY(y);
}
