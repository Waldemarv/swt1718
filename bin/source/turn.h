#ifndef TURN_H
#define TURN_H

#include "tile.h"
#include <QString>

class turn : public Tile
{
public:
    turn();
    QString getType();
    void drawTile();
};

#endif // TURN_H
