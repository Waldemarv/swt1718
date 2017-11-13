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

    //Hightligh when Selected
    if(selected){
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(3);
        painter->setPen(pen);
        painter->drawRect(boundingRect());
    }
    // Change Pen: Color to Black and Size to 3
    QPen mainPen;
    mainPen.setColor(Qt::black);
    mainPen.setWidth(1);
    painter->setPen(mainPen);

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
    pressed = false;
    selected = false;

}
Obstacle::Obstacle(double x, double y, double width, double length) : width(width), length(length)
{
    position = new Point(x,y);
    setFlag(QGraphicsItem::ItemIsMovable);
    pressed = false;
    selected = false;
    //setPos(x,y);
}

void Obstacle::setPosition(double x, double y)
{
    position->setX(x);
    position->setY(y);
    //setPos(x,y);
}

// Mouse Event Handlers

void Obstacle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed=true;

    if(selected){
       selected = false;
    } else {
       selected = true;
    }

    update();
    QGraphicsItem::mousePressEvent(event);
}

void Obstacle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed=false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

