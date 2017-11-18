#ifndef STARTINGTILE_H
#define STARTINGTILE_H

#include "tile.h"

class Startingtile : public Tile
{
private:
    Startingtile();
    static Startingtile* startingTilePointer;

public:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString getType();

    static Startingtile* createStartingTile();

    void rotate();
};

#endif // STARTINGTILE_H
