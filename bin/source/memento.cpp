#include "memento.h"

Memento::Memento()
{

}
/*! Erzeugt ein Memento mit allen relevanten Daten der Map
 * \param ntiles Tile-Vektor der Map
 * \param nobstacles Obstacle-Vektor der Map
 * \param ngridSize GridSize der Map
 * \param nsizeX Größe der Map in x-Richtung
 * \param nsizeY Größe der Map in y-Richtung
 * \param nstartingPoint Startpunkt der Map
 * \param nendingPoint Endpunkt der Map
 * \param nstartingTile Starttile der Map
 * \param nendingTile Endtile der Map */
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
