#include "straight.h"

straight::straight()
{
    Tile();
}

straight::straight(double x, double y, double ascent)
{
    position->setX(x);
    position->setY(y);
    setAscent(ascent);
    setPos(x,y);

    QRectF rec = boundingRect();
    topLeft = rec.topLeft();
    topRight = rec.topRight();
    bottomLeft = rec.bottomLeft();
    bottomRight = rec.bottomRight();
}

QRectF straight::boundingRect() const
{
    return QRectF(position->getX(),position->getY(),50,50);
}

void straight::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawLine(topLeft, topRight);
    painter->drawLine(bottomLeft,bottomRight);
}

void straight::rotate()
{
    QPointF temp = topLeft;
    topLeft = topRight;
    topRight = bottomRight;
    bottomRight = bottomLeft;
    bottomLeft = temp;

    update();
}

QString straight::getType()
{
    return "straight";
}
