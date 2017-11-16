#include "turn.h"

turn::turn()
{
}

/*! Erstellt eine Kurve
 * \param x x-Position der Kurve
 * \param y y-Position der Kurve
 * \param ascent Steigung der Kurve */
turn::turn(double x, double y, double ascent)
{
    position->setX(x);
    position->setY(y);
    setAscent(ascent);

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
/*! Erstellt ein Begrenzungsrechteck für das Tile,Dieses wird sowohl zum zeichnen, als auch für weitere Interaktion benötigt */
QRectF turn::boundingRect() const
{
    return QRectF(position->getX(),position->getY(),100,100);
}

/*! Zeichnet das Tile
 * \param painter Painter der zum Zeichnen benutzt wird
 * \param option Optionen für das Zeichnen
 * \param widget Widget in welches gezeichnet wird */
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
    mainPen.setWidth(2);
    painter->setPen(mainPen);

    QRectF rec = boundingRect();
    QBrush brush(Qt::red);

    painter->drawLine(topLeft, topRight);
    painter->drawLine(topRight, bottomRight);

    painter->drawLine(rec.center(), leftCenter);
    painter->drawLine(rec.center(), bottomCenter);
}

/*! Rotiert das Tile */
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

/*! Gibt den Typen des Tile zurück
 * \return Typ des Tile */
QString turn::getType()
{
    return "turn";
}

