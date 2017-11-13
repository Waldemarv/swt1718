#include "turn.h"

turn::turn()
{
}

turn::turn(double x, double y, double ascent)
{
    position->setX(x);
    position->setY(y);
    setAscent(ascent);

    setPos(x,y);

    QRectF rec = boundingRect();
    topLeft = rec.topLeft();
    topRight = rec.topRight();
    bottomLeft = rec.bottomLeft();
    bottomRight = rec.bottomRight();
    topCenter = QPointF(rec.center().x(), rec.center().y()-50);
    bottomCenter = QPointF(rec.center().x(), rec.center().y()+50);
    leftCenter = QPointF(rec.center().x()-50, rec.center().y());
    rightCenter = QPointF(rec.center().x()+50, rec.center().y());

}

QRectF turn::boundingRect() const
{
    return QRectF(position->getX(),position->getY(),100,100);
}

void turn::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
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

    QRectF rec = boundingRect();
    QBrush brush(Qt::red);

    painter->drawLine(topLeft, topRight);
    painter->drawLine(topRight, bottomRight);

    painter->drawLine(rec.center(), leftCenter);
    painter->drawLine(rec.center(), bottomCenter);
}

void turn::rotate()
{
    QPointF temp = topLeft;
    topLeft = topRight;
    topRight = bottomRight;
    bottomRight = bottomLeft;
    bottomLeft = temp;

    temp=leftCenter;
    leftCenter = topCenter;
    topCenter = rightCenter;
    rightCenter = bottomCenter;
    bottomCenter = temp;

    update();
}

QString turn::getType()
{
    return "turn";
}

