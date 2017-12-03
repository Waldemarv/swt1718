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
    QColor color;

public:
    Sensor();
    Sensor(int direction, QPointF p);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int getAngle();
    int getDirection();

    void setColor(QColor c);
};

#endif // SENSOR_H