#ifndef STRAIGHT_H
#define STRAIGHT_H

#include "tile.h"
#include <QString>

class straight : public Tile
{
public:
    straight();
    straight(double x, double y, double ascent);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QString getType();

    void rotate();
};

#endif // STRAIGHT_H
