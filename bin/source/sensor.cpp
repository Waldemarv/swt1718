#include "sensor.h"

Sensor::Sensor() : direction(0)
{}
/*! Erstellt einen neuen Sensor
 * \param nangle Winkel des Sensors
 * \param nlength Länge des Sensors
 * \param p Punkt an dem der Sensor beginnen soll */
Sensor::Sensor(int nangle, int nlength, QPointF p) : direction(direction), angle(nangle), length(nlength)
{
    setPos(p);

    setColor(Qt::green);

    setRotation(angle);
}
/*! Erstellt ein BoundingRect für den Sensor */
QRectF Sensor::boundingRect() const
{
    return QRectF(0,0,length,2);
}

/*! Zeichnet den Sensor */
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

int Sensor::getLength()
{
    return length;
}

void Sensor::setColor(QColor c)
{
    color = c;
}

void Sensor::setLength(int nlength)
{
    length = nlength;
}

void Sensor::setAngle(int nangle)
{
    angle = nangle;
}

void Sensor::resetRotation()
{
    setRotation(angle);
}
