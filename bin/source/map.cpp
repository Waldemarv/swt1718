#include "map.h"
#include "point.h"
#include <iostream>
using namespace std;

Map::Map()
{
    //TODO Vektoren
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
