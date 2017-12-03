#include "sensor.h"

Sensor::Sensor() : direction(0)
{}

Sensor::Sensor(int nangle, QPointF p) : direction(direction)
{
    setPos(p);

    setColor(Qt::green);

    angle = nangle;
    setRotation(angle);
}
QRectF Sensor::boundingRect() const
{
    return QRectF(0,0,20,2);
}

void Sensor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath *newPath = new QPainterPath;

    QPen pen;
    pen.setColor(color);
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

void Sensor::setColor(QColor c)
{
    color = c;
}
