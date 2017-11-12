#ifndef MAP_H
#define MAP_H
#include "point.h"
#include "tile.h"
#include "straight.h"
#include "turn.h"
#include "obstacle.h"
#include <vector>

class Map
{
private:
    std::vector<Tile*> tiles;
    std::vector<Obstacle*> obstacles;

    int sizeX;
    int sizeY;

    Point startingPoint;
    Point endingPoint;

public:
    Map();
    Map(int x, int y);
    ~Map();

    Tile* getTile(int i);
    Tile* getCurrentTile();
    Obstacle* getObstacle(int i);
    Obstacle* getCurrentObstacle();

    void setStartingPoint(int x, int y);
    void setEndingPoint(int x, int y);
    void addTile(Tile* t);
    void addObstacle(Obstacle* o);
    void deleteCurrentTile();
    void deleteCurrentObstacle();

    Point getStartingPoint();
    Point getEndingPoint();

    unsigned int getNumberOfTiles();
    unsigned int getNumberOfObstacles();
    unsigned int getNumberOfItems();

    //TODO createMemento
    //TODO setMemento
};

#endif // MAP_H
