#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "point.h"
#include <QString>

class Obstacle {

private:
	Point position;
	double width;
	double length;

public:
    double getwidth();
    double getlength();

    virtual void drawObstacle();
    virtual QString getType();

    Obstacle();
    Obstacle(double x, double y, double width, double length);

    Point getPosition();
};

#endif
