#ifndef SMARTVEHICLE_H
#define SMARTVEHICLE_H

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

    QColor color;

    void collision();

public:
    SmartVehicle();
    SmartVehicle(int nangle, int nspeed, int nrotationSpeed, int x, int y);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void left();
    void right();

    void setColor(QColor c);

protected:
    void advance(int phase);
};

#endif // SMARTVEHICLE_H
