#ifndef SMARTVEHICLE_H
#define SMARTVEHICLE_H

#include "sensor.h"
#include <vector>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>

class SmartVehicle : public QGraphicsItem
{
private:
    int angle;
    int rotationSpeed;
    int speed;

    std::vector<Sensor*> sensors;

    QColor color;

public:
    SmartVehicle();
    SmartVehicle(int nangle, int nspeed, int nrotationSpeed, int x, int y);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    Sensor* getSensor(int i);

    void left();
    void right();

    void setColor(QColor c);

    int getNumberOfSensors();
protected:
    void advance(int phase);
};

#endif // SMARTVEHICLE_H
