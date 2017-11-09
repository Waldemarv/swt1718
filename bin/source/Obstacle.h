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
};

#endif
