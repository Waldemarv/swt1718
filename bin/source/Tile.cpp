#include "Tile.h"

void Tile::drawTile() {
}

Tile Tile::rotate(Tile t) {
}

Tile::Tile() {
}

Tile::Tile(double X, double Y, double Ascent) : ascent(Ascent) {
    position = new Point(X,Y);
}

double Tile::getAscent(){ return ascent; }

Point Tile::getPosition() { return position; }
