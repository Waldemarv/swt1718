#ifndef STARTINGTILE_H
#define STARTINGTILE_H

#include "tile.h"

class Startingtile : public Tile
{
private:
    Startingtile();
    Startingtile(int nx, int ny, double nascent, int ndirection);
    ~Startingtile();
    static Startingtile* startingTilePointer;

public:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString getType();

    static Startingtile* createStartingTile(int nx, int ny, double nascent, int ndirection);

    void rotate();
};

#endif // STARTINGTILE_H
