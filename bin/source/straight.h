#ifndef STRAIGHT_H
#define STRAIGHT_H

#include "tile.h"
#include <QString>

class straight : public Tile
{
public:
    straight();
    QString getType();
    void drawTile();
};

#endif // STRAIGHT_H
