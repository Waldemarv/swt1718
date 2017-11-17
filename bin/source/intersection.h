#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "tile.h"
#include <QString>

class Intersection : public Tile
{
public:
    Intersection();
    Intersection(double x, double y, double ascent);

    QRectF boundingRect() const;
    QString getType();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // INTERSECTION_H
