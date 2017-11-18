#include "endingtile.h"

Endingtile::Endingtile()
{
    QRectF rec = boundingRect();
    topLeft = rec.topLeft();
    topRight = rec.topRight();
    bottomLeft = rec.bottomLeft();
    bottomRight = rec.bottomRight();
}

QRectF Endingtile::boundingRect() const
{
    return QRectF(0,0,50,50);
}

QString Endingtile::getType()
{
    return "EndingTile";
}

Endingtile *Endingtile::createEndingTile()
{
    if(endingTilePointer == 0)
    {
        endingTilePointer = new Endingtile();
    }
    return endingTilePointer;
}

void Endingtile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    mainPen.setColor(Qt::magenta);
    mainPen.setWidth(2);
    painter->setPen(mainPen);

    painter->drawLine(topLeft, topRight);
    painter->drawLine(bottomLeft,bottomRight);
    painter->drawLine(topRight, bottomRight);
}

void Endingtile::rotate()
{
    QPointF temp = topLeft;
    topLeft = topRight;
    topRight = bottomRight;
    bottomRight = bottomLeft;
    bottomLeft = temp;

    update();
}


Endingtile* Endingtile::endingTilePointer = 0;
