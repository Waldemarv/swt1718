#ifndef STRAIGHT_H
#define STRAIGHT_H

#include "tile.h"
#include <QString>
/*! Unterklasse von Tile, realisiert ein gerades Streckenstück*/
class straight : public Tile
{
public:
    straight();
    straight(double nx, double ny, double nascent, int ndirection);

    QRectF boundingRect() const;
    QString getType();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void rotate();
};

#endif // STRAIGHT_H
