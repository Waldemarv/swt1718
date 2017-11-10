#ifndef TILE_H
#define TILE_H

#include "point.h"
#include <QString>

class Tile {

private:
	Point position;
	double ascent;

public:
    double getAscent();

    virtual void drawTile();
    virtual QString getType();

    Point getPosition();

    Tile rotate(Tile t);
    Tile();
    Tile(double x, double y, double ascent);
};

#endif
