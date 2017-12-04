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
    int length;
    QPainterPath path;
    QColor color;

public:
    Sensor();
    Sensor(int nangle, int nlength, QPointF p);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int getAngle();

    void setColor(QColor c);
};

#endif // SENSOR_H
