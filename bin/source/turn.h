#ifndef TURN_H
#define TURN_H

#include "tile.h"
#include <QString>

class turn : public Tile
{
public:
    turn();
    turn(double nx, double ny, double nascent, int ndirection);

    QRectF boundingRect() const;
    QString getType();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void rotate();
};



#endif // TURN_H
