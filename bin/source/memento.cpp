#include "memento.h"

Memento::Memento()
{

}

Memento::Memento(std::vector<Tile *> ntiles, std::vector<Obstacle *> nobstacles, int ngridSize, int nsizeX, int nsizeY, QPointF nstartingPoint, QPointF nendingPoint, Tile *nstartingTile, Tile *nendingTile)
{
    for(int i = 0; i<ntiles.size(); i++)
        tilesPositions.push_back(ntiles.at(i)->pos());
    for(int i = 0; i<nobstacles.size(); i++)
        obstaclesPositions.push_back(nobstacles.at(i)->pos());

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
