#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "point.h"

class Obstacle {

private:
	Point position;
	double width;
	double length;

public:
	void drawObstacle();
    double getwidth();
    double getlength();

    Obstacle();
    Obstacle(double x, double y, double width, double length);

    Point getPosition();
};

#endif
