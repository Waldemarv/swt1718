#ifndef TILE_H
#define TILE_H

#include "point.h"

class Tile {

private:
	Point position;
	double ascent;

public:
	void drawTile();

    double getAscent();

    Point getPosition();

    Tile rotate(Tile t);
	Tile();
    Tile(double x, double y, double ascent);
};

#endif
