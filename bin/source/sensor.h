#ifndef SENSOR_H
#define SENSOR_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <QBrush>

class Sensor : public QGraphicsItem
{
private:
    int direction;
    int angle;
    QPainterPath path;

public:
    Sensor();
    Sensor(int direction, QPointF p);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int getAngle();
    int getDirection();
};

#endif // SENSOR_H
