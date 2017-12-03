#include "sensor.h"

Sensor::Sensor() : direction(0)
{}

Sensor::Sensor(int direction, QPointF p) : direction(direction)
{
    setPos(p);

    if(direction == 0)
    {
        //Sensor vorne rechts
        angle = 45;
        setRotation(angle);
    }
    else if(direction == 1)
    {
        //Sensor vorne Links
        angle = -45;
        setRotation(angle);
    }
}

QRectF Sensor::boundingRect() const
{
    return QRectF(0,0,15,15);
}

void Sensor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath *newPath = new QPainterPath;

    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(2);
    painter->setPen(pen);
    newPath->moveTo(boundingRect().topLeft());
    newPath->lineTo(boundingRect().topRight());
    path = *newPath;
    painter->drawPath(*newPath);
}

int Sensor::getAngle()
{
    return angle;
}

int Sensor::getDirection()
{
    return direction;
}
