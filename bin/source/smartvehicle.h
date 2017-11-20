#ifndef SMARTVEHICLE_H
#define SMARTVEHICLE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>

class SmartVehicle : public QGraphicsItem
{
private:
    int angle;
    int rotationSpeed;
    int speed;

    void collision();

public:
    SmartVehicle();
    SmartVehicle(int nangle, int nspeed, int nrotationSpeed, int x, int y);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void left();
    void right();

protected:
    void advance(int phase);
};

#endif // SMARTVEHICLE_H
