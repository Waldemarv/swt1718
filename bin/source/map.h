#ifndef MAP_H
#define MAP_H
#include "point.h"
#include "tile.h"
#include "straight.h"
#include "turn.h"
#include "obstacle.h"
#include "intersection.h"
#include "tintersection.h"
#include "startingtile.h"
#include "endingtile.h"
#include "smartvehicle.h"
#include <vector>


/*! Speichert die relevanten Daten einer Map und erlaubt das hinzufügen und bearbeiten von Elementen auf dieser */
class Map
{
private:
    std::vector<Tile*> tiles;
    std::vector<Obstacle*> obstacles;

    int gridSize;

    int sizeX;
    int sizeY;

    Point startingPoint;
    Point endingPoint;

    Tile* startingTile;
    Tile* endingTile;

public:
    Map();
    Map(int x, int y);
    ~Map();

    Tile* getTile(int i);
    Tile* getCurrentTile();
    Obstacle* getObstacle(int i);
    Obstacle* getCurrentObstacle();

    Tile* getStartingTile();
    Tile* getEndingTile();

    void setStartingTile(Tile* t);
    void setEndingTile(Tile* t);

    void setStartingPoint(int x, int y);
    void setEndingPoint(int x, int y);
    void setSize(int x, int y);
    void addTile(Tile* t);
    void addObstacle(Obstacle* o);

    void deleteTile(int i);
    void deleteObstacle(int i);

    void deleteCurrentTile();
    void deleteCurrentObstacle();

    Point getStartingPoint();
    Point getEndingPoint();

    unsigned int getNumberOfTiles();
    unsigned int getNumberOfObstacles();
    unsigned int getNumberOfItems();

    unsigned int getSizeX();
    unsigned int getSizeY();
    int getGridSize();

    //TODO createMemento
    //TODO setMemento
};

#endif // MAP_H
