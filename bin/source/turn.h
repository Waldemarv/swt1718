#ifndef TURN_H
#define TURN_H

#include "tile.h"
#include <QString>

class turn : public Tile
{
protected :
    int direction;

public:
    turn();
    turn(double x, double y, double ascent);

    QRectF boundingRect() const;
    QString getType();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void rotate();
};

#endif // TURN_H
