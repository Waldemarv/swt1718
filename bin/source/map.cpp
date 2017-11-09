#include "map.h"

Map::Map()
{
    this->sizeX=0;
    this->sizeY=0;
    this->startingPoint = new Point();
    this->endingPoint = new Point();
}

Map::Map(int x, int y)
{
    this->sizeX = x;
    this->sizeY = y;
}

unsigned int Map::getNumberOfTiles() { return tiles.size(); }
unsigned int Map::getNumberOfObstacles() { return obstacles.size(); }

double Map::getStartingPointX() { return startingPoint.getX(); }
double Map::getStartingPointY() { return startingPoint.getY(); }
double Map::getEndingPointX() { return endingPoint.getX(); }
double Map::getEndingPointY() { return endingPoint.getY(); }

Tile Map::getTile(int i) { return tiles[i]; }
Obstacle Map::getObstacle(int i) { return obstacles[i]; }
