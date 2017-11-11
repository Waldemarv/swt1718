#ifndef TURN_H
#define TURN_H

#include "tile.h"
#include <QString>

class turn : public Tile
{
public:
    turn();
    turn(double x, double y, double ascent);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QString getType();
};

#endif // TURN_H
