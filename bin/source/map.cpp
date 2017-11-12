#include "map.h"

Map::Map()
{
    sizeX=0;
    sizeY=0;
}

Map::Map(int x, int y)
{
    sizeX = x;
    sizeY = y;
}

unsigned int Map::getNumberOfTiles() { return tiles.size(); }
unsigned int Map::getNumberOfObstacles() { return obstacles.size(); }
unsigned int Map::getNumberOfItems() { return obstacles.size() + tiles.size(); } //TODO: Erweitern für alle Objekte

void Map::setStartingPoint(int x, int y) { startingPoint = new Point(x,y); }
void Map::setEndingPoint(int x, int y) { endingPoint = new Point(x,y); }
void Map::addTile(Tile* t) { tiles.push_back(t); }
void Map::addObstacle(Obstacle* o) { obstacles.push_back(o); }
void Map::deleteCurrentTile() { tiles.pop_back(); }
void Map::deleteCurrentObstacle() { obstacles.pop_back(); }

Point Map::getStartingPoint() { return startingPoint; }
Point Map::getEndingPoint() { return endingPoint; }

Tile* Map::getTile(int i) { return tiles[i]; }
Tile *Map::getCurrentTile() { return tiles.back(); }

Obstacle* Map::getObstacle(int i) { return obstacles[i]; }
Obstacle *Map::getCurrentObstacle() { return obstacles.back(); }
