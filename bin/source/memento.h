#ifndef MEMENTO_H
#define MEMENTO_H

#include <vector>
#include <QPointF>
#include "tile.h"
#include "obstacle.h"

class Memento
{
public:
    Memento();
    Memento(std::vector<Tile*> ntiles, std::vector<Obstacle*> nobstacles, int ngridSize, int nsizeX, int nsizeY, QPointF nstartingPoint, QPointF nendingPoint, Tile* nstartingTile, Tile* nendingTile);

    std::vector<Tile*> tiles;
    std::vector<Obstacle*> obstacles;
    std::vector<QPointF> tilesPositions;
    std::vector<QPointF> obstaclesPositions;

    int gridSize;

    int sizeX;
    int sizeY;

    QPointF startingPoint;
    QPointF endingPoint;

    Tile* startingTile;
    Tile* endingTile;
};

#endif // MEMENTO_H
