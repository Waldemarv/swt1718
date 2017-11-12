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
    QString getType();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void rotate();
};

#endif // STRAIGHT_H
