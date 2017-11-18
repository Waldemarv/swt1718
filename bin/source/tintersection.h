#ifndef TINTERSECTION_H
#define TINTERSECTION_H

#include "tile.h"
#include <QString>

class Tintersection : public Tile
{
public:
    Tintersection();
    Tintersection(double x, double y, double ascent);

    QRectF boundingRect() const;
    QString getType();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void rotate();
};

#endif // TINTERSECTION_H
