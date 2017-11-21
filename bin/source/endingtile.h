#ifndef ENDINGTILE_H
#define ENDINGTILE_H

#include "tile.h"

class Endingtile : public Tile
{
private:
    Endingtile();
    Endingtile(int nx, int ny, double nascent, int ndirection);
    ~Endingtile();
    static Endingtile* endingTilePointer;

public:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString getType();

    static Endingtile* createEndingTile(int nx, int ny, double nascent, int ndirection);

    void rotate();
};

#endif // Endingtile_H
