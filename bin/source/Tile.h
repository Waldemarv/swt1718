#ifndef TILE_H
#define TILE_H

#include "point.h"

class Tile {

private:
	Point position;
	bool openEnds[4];
	double ascent;

public:
	void drawTile();

    double getAscent();

    Tile rotate(Tile t);

	Tile();
};

#endif
