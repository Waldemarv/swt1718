#include "memento.h"

Memento::Memento()
{

}

Memento::Memento(std::vector<Tile *> ntiles, std::vector<Obstacle *> nobstacles, int ngridSize, int nsizeX, int nsizeY, QPointF nstartingPoint, QPointF nendingPoint, Tile *nstartingTile, Tile *nendingTile)
{
    tiles = ntiles;
    obstacles = nobstacles;
    gridSize = ngridSize;
    sizeX = nsizeX;
    sizeY = nsizeY;
    startingPoint = nstartingPoint;
    endingPoint = nendingPoint;
    startingTile = nstartingTile;
    endingTile = nendingTile;
}
