#include "obstacle.h"

QString Obstacle::getType()
{
    return "staticObstacle";
}

QRectF Obstacle::boundingRect() const
{
    return QRectF(position->getX(),position->getY(),width,length);
}

void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Nur zum Testen
    QRectF rec = boundingRect();
    QBrush brush(Qt::blue);
    painter->fillRect(rec, brush);
    painter->drawRect(rec);
}


double Obstacle::getlength() { return length; }
double Obstacle::getwidth() {return width; }

Point* Obstacle::getPosition() { return position; }

Obstacle::Obstacle(){
    position = new Point();
    setFlag(QGraphicsItem::ItemIsMovable);

}
Obstacle::Obstacle(double x, double y, double width, double length) : width(width), length(length)
{
    position = new Point(x,y);
    setFlag(QGraphicsItem::ItemIsMovable);
    //setPos(x,y);
}

void Obstacle::setPosition(double x, double y)
{
    position->setX(x);
    position->setY(y);
    //setPos(x,y);
}
