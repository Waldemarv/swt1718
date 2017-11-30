#include "map.h"
/*! Erstellt eine Map der größe 0 */
Map::Map()
{
    sizeX=0;
    sizeY=0;

    gridSize = 25;
}
/*! Erstellt eine leere Map
    \param x Größe der Map in x-Richtung
    \param y Größe der Map in y-Richtung
*/
Map::Map(int x, int y)
{
    sizeX = x;
    sizeY = y;

    gridSize = 50;
}
/*! Destruktor für die Map. Es werden erst alle Elememte auf der Map gelöscht, dann die Map selbst */
Map::~Map()
{
    for (auto it = tiles.begin() ; it != tiles.end(); ++it)
        delete (*it);
    for(auto it = obstacles.begin() ; it != obstacles.end(); ++it)      //Erweitern für SmartVehicle etc.
        delete (*it);

    tiles.clear();
    obstacles.clear();

    delete &endingPoint;
    delete &startingPoint;
}
/*! Gibt die Anzahl der Tiles auf der Map zurück
    \return Anzahl der Tiles auf der Map*/
unsigned int Map::getNumberOfTiles() { return tiles.size(); }
/*! Gibt die Anzahl der Obstacles auf der Map zurück
 *  \return Anzahl der Obstacles auf der Map */
unsigned int Map::getNumberOfObstacles() { return obstacles.size(); }
/*! Gibt die Gesamtanzahl der Elemente auf der Map zurück
 *  \return Gesamtzahl = Anzahl der Tiles + Anzahl der Obstacles +... */
unsigned int Map::getNumberOfItems() { return obstacles.size() + tiles.size(); } //TODO: Erweitern für alle Objekte

/*! Gibt die Größe in x-Richtung der Map zurück
 * \return Größe der Map in x-Richtung */
unsigned int Map::getSizeX() { return sizeX; }
/*! Gibt die Größe in y-Richtung der Map zurück
 * \return Größe der Map in y-Richtung */
unsigned int Map::getSizeY() { return sizeY; }

/*! Gibt die Rastergröße der Map zurück
 * \return Rastergröße der Map */
int Map::getGridSize() { return gridSize; }

/*! Setzt den Startpunkt des Fahrzeuges
 * \param x x-Koordinate des Startpunktes
 * \param y y-Koordinate des Startpunktes */
void Map::setStartingPoint(int x, int y) { startingPoint.setX(x); startingPoint.setY(y); }
/*! Setzt den Endpunkt des Fahrzeuges
 * \param x x-Koordinate des Endpunktes
 * \param y y-Koordinate des Endpunktes */
void Map::setEndingPoint(int x, int y) { endingPoint.setX(x); endingPoint.setY(y); }

/*! Verändert die Größe einer Map
 * \param x neue Größe der Map in x-Richtung
 * \param y neue Größe der Map in y-Richtung */
void Map::setSize(int x, int y)
{
    sizeX=x;
    sizeY=y;
}

/*! Fügt ein Tile dem Tile-Vektor hinzu
 * \param t Tile, welches hinzugefügt wird */
void Map::addTile(Tile* t) { tiles.push_back(t); }
/*! Fügt ein Obstacle dem Obstacle-Vektor hinzu
 * \param o Obstacle, welches hinzugefügt wird */
void Map::addObstacle(Obstacle* o) { obstacles.push_back(o); }

/*! Löscht das letzte erstellte Tile */
void Map::deleteCurrentTile() { tiles.pop_back(); }
/*! Löscht das letzte erstellte Obstacle */
void Map::deleteCurrentObstacle() { obstacles.pop_back(); }

/*! Löscht ein gewähltes Tile
 * \param i Position des Tiles im Tile-Vektor */
void Map::deleteTile(int i){    tiles.erase(tiles.begin()+i);   }
/*! Löscht ein gewähltes Obstacle
 * \param i Position des Obstacles im Obstacle-Vektor */
void Map::deleteObstacle(int i) {   obstacles.erase(obstacles.begin()+i); }

/*! Gibt den Startpunkt des autonomen Fahrzeugs zurück
 * \return Startpunkt des Fahrzeuges */
QPointF Map::getStartingPoint() { return startingPoint; }
/*! Gibt den Endpunkt des autonomen Fahrzeugs zurück
 * \return Endpunkt des Fahrzeuges */
QPointF Map::getEndingPoint() { return endingPoint; }

/*! Gibt gewähltes Tile zurück
 * \param i Position des Tiles im Tile-Vektor
 * \return Tile an Position i im Tile-Vektor */
Tile* Map::getTile(int i) { return tiles[i]; }

/*! Gibt das letzte erstellte Tile zurück
 * \return Tile, welches zuletzt erstellt wurde */
Tile *Map::getCurrentTile() { return tiles.back(); }

/*! Gibt gewähltes Obstacle zurück
 * \param i Position des Obstacles im Obstacle-Vektor
 * \return Obstacle an Position i im Obstacle-Vektor */
Obstacle* Map::getObstacle(int i) { return obstacles[i]; }

/*! Gibt das letzte erstellte Obstacle zurück
 * \return Obstacle,welches zuletzt erstellt wurde */
Obstacle *Map::getCurrentObstacle() { return obstacles.back(); }

Tile *Map::getStartingTile()
{
    return startingTile;
}

Tile *Map::getEndingTile()
{
    return endingTile;
}

void Map::setStartingTile(Tile *t)
{
    startingTile = t;
}

void Map::setEndingTile(Tile *t)
{
    endingTile = t;
}

/*! Fügt die einzelnen Begrenzungen der Tiles zu einem Gesamtpfad zusammen */
void Map::setMapPath() {
    QPainterPath newMapPath;

    for(unsigned int i = 0; i<tiles.size(); i++) {

        tiles.at(i)->setPath(tiles.at(i)->mapToScene(tiles.at(i)->getPath()));

        newMapPath.addPath(tiles.at(i)->getPath());
    }
    this->mapPath=newMapPath;
}

/*! Gibt die Begrenzungen der Map zurück */
QPainterPath Map::getMapPath() {
    return this->mapPath;
}

Memento *Map::createMemento()
{
    return new Memento(tiles, obstacles, gridSize, sizeX, sizeY, startingPoint, endingPoint, startingTile, endingTile);
}

void Map::setMemento(Memento *m)
{
    tiles = m->tiles;
    obstacles = m->obstacles;
    gridSize = m->gridSize;
    sizeX = m->sizeX;
    sizeY = m->sizeY;
    startingPoint = m->startingPoint;
    endingPoint = m->endingPoint;
    startingTile = m->startingTile;
    endingTile = m->endingTile;
}
