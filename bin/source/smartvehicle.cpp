#include "smartvehicle.h"

void SmartVehicle::collision()
{
}

SmartVehicle::SmartVehicle()
{
    angle = 0;
    setRotation(angle);
    speed = 1;
    setPos(100,100);
}

SmartVehicle::SmartVehicle(int nangle, int nspeed, int nrotationSpeed, int x, int y)
{
    angle = nangle%360;
    setRotation(angle);
    speed = nspeed;
    qDebug()<<"speed: "<<speed;
    rotationSpeed = nrotationSpeed;
    qDebug()<<"rotation speed: "<<rotationSpeed;
    setPos(x,y);

    color = Qt::green;
}

QRectF SmartVehicle::boundingRect() const
{
    return QRectF(0,0,15,10);
}

void SmartVehicle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{   
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QBrush brush (color);
    QRectF rec = boundingRect();

    painter->fillRect(rec, brush);
    painter->drawRect(rec);
}

void SmartVehicle::right()
{
    angle+=rotationSpeed;
    setRotation(angle);
}

void SmartVehicle::setColor(QColor c)
{
    color = c;
    update();
}

void SmartVehicle::left()
{
    angle-=rotationSpeed;
    setRotation(angle);
}

void SmartVehicle::advance(int phase)
{
    if(!phase)
        return;
    setPos(mapToParent(speed, 0));
}
