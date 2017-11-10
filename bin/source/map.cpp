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

void Map::setStartingPoint(int x, int y) { startingPoint = new Point(x,y); }
void Map::setEndingPoint(int x, int y) { endingPoint = new Point(x,y); }
void Map::addTile(Tile* t) { tiles.push_back(t); }
void Map::addObstacle(Obstacle* o) { obstacles.push_back(o); }

Point Map::getStartingPoint() { return startingPoint; }
Point Map::getEndingPoint() { return endingPoint; }

Tile* Map::getTile(int i) { return tiles[i]; }
Obstacle* Map::getObstacle(int i) { return obstacles[i]; }
