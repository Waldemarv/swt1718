#include "smartvehicle.h"

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
    rotationSpeed = nrotationSpeed;
    setPos(x,y);

    /*Front sensors*/
    //Front right
    Sensor* r = new Sensor(50, 20, mapToScene(boundingRect().bottomRight()));
    sensors.push_back(r);
    //Front left
    Sensor* l = new Sensor(-50,20, mapToScene(boundingRect().topRight()));
    sensors.push_back(l);
    //Front mid
    Sensor* m = new Sensor(0,20, mapToScene(QPointF(boundingRect().center().x(), boundingRect().right())));
    sensors.push_back(m);

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

Sensor *SmartVehicle::getSensor(int i)
{
    return sensors.at(i);
}

void SmartVehicle::right()
{
    angle+=rotationSpeed;
    setRotation(angle);

    for(int i = 0; i<sensors.size();i++)
    {
        switch(i)
        {
            //Front Right Sensor
            case 0: sensors.at(i)->setPos(mapToScene(boundingRect().bottomRight())); break;
            //Front Left Sensor
            case 1: sensors.at(i)->setPos(mapToScene(boundingRect().topRight())); break;
            //Front Mid Sensor
            case 2: sensors.at(i)->setPos(mapToScene(QPointF(boundingRect().right(), boundingRect().center().y()))); break;
        }

        sensors.at(i)->setRotation(angle+sensors.at(i)->getAngle());
    }
}

void SmartVehicle::setColor(QColor c)
{
    color = c;
    update();
}

int SmartVehicle::getNumberOfSensors()
{
    return sensors.size();
}

void SmartVehicle::left()
{
    angle-=rotationSpeed;
    setRotation(angle);

    for(int i = 0; i<sensors.size();i++)
    {
        switch(i)
        {
            //Front Right Sensor
            case 0: sensors.at(i)->setPos(mapToScene(boundingRect().bottomRight())); break;
            //Front Left Sensor
            case 1: sensors.at(i)->setPos(mapToScene(boundingRect().topRight())); break;
            //Front Mid Sensor
            case 2: sensors.at(i)->setPos(mapToScene(QPointF(boundingRect().right(), boundingRect().center().y()))); break;
        }
        sensors.at(i)->setRotation(angle+sensors.at(i)->getAngle());
    }
}

void SmartVehicle::advance(int phase)
{
    if(!phase)
        return;
    setPos(mapToParent(speed, 0));

    for(int i = 0; i<sensors.size();i++)
    {
        switch(i)
        {
            //Front Right Sensor
            case 0: sensors.at(i)->setPos(mapToScene(boundingRect().bottomRight())); break;
            //Front Left Sensor
            case 1: sensors.at(i)->setPos(mapToScene(boundingRect().topRight())); break;
            //Front Mid Sensor
            case 2: sensors.at(i)->setPos(mapToScene(QPointF(boundingRect().right(), boundingRect().center().y()))); break;
        }
    }
}
