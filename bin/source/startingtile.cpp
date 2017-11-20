#include "startingtile.h"

Startingtile::Startingtile()
{
    QRectF rec = boundingRect();
    topLeft = rec.topLeft();
    topRight = rec.topRight();
    bottomLeft = rec.bottomLeft();
    bottomRight = rec.bottomRight();
}

Startingtile::~Startingtile()
{
    startingTilePointer = 0;
}

QRectF Startingtile::boundingRect() const
{
    return QRectF(0,0,50,50);
}

QString Startingtile::getType()
{
    return "StartingTile";
}

Startingtile *Startingtile::createStartingTile()
{
    if(startingTilePointer == 0)
    {
        qDebug()<<"Kein Tile";
        startingTilePointer = new Startingtile();
    }
    qDebug()<<"Ein Tile";
    return startingTilePointer;
}

void Startingtile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Hightlight when Selected
    if(selected){
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(3);
        painter->setPen(pen);
        painter->drawRect(boundingRect());
    }

    QPen mainPen;
    mainPen.setColor(Qt::green);
    mainPen.setWidth(2);
    painter->setPen(mainPen);

    painter->drawLine(topLeft, topRight);
    painter->drawLine(bottomLeft,bottomRight);
    painter->drawLine(topLeft, bottomLeft);
}

void Startingtile::rotate()
{
    QPointF temp = topLeft;
    topLeft = topRight;
    topRight = bottomRight;
    bottomRight = bottomLeft;
    bottomLeft = temp;

    update();
}

Startingtile* Startingtile::startingTilePointer = 0;
