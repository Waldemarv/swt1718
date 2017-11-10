#include "Tile.h"

void Tile::drawTile() {
}

Tile Tile::rotate(Tile t) {
}

Tile::Tile() {
}

Tile::Tile(double x, double y, double Ascent) : ascent(Ascent) {
    position.setX(x);
    position.setY(y);
}

double Tile::getAscent(){ return ascent; }

Point Tile::getPosition() { return position; }
