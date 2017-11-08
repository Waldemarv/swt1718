#ifndef MAP_H
#define MAP_H
#include "point.h"

class Map
{
private:
    //TODO obstacleVector
    //TODO tileVector
    int sizeX;
    int sizeY;
    Point startingPoint;
    Point endingPoint;
public:
    Map();
    Map(int x, int y);
    //TODO createMemento
    //TODO setMemento
};

#endif // MAP_H
