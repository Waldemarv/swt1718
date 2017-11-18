#ifndef ENDINGTILE_H
#define ENDINGTILE_H

#include "tile.h"

class Endingtile : public Tile
{
private:
    Endingtile();
    static Endingtile* endingTilePointer;

public:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString getType();

    static Endingtile* createEndingTile();

    void rotate();
};

#endif // Endingtile_H
