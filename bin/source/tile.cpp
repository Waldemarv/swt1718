#include "tile.h"

Tile::Tile() {
}

Tile::Tile(double x, double y, double Ascent) : ascent(Ascent) {
    position.setX(x);
    position.setY(y);
}

void Tile::drawTile() {
}

Tile Tile::rotate(Tile t) {
}

double Tile::getAscent(){ return ascent; }

QString Tile::getType()
{
    return "tile";
}

Point Tile::getPosition() { return position; }
