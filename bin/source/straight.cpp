#include "straight.h"

/*! Erstellt ein leeres gerades Streckenstück */
straight::straight()
{
}

/*! Erstellt ein gerades Streckenstück
 * \param x x-Koordinate der Geraden
 * \param y y-Koordinate der Geraden
 * \param ascent Steigung der Geraden */
straight::straight(double x, double y, double ascent)
{
    position->setX(x);
    position->setY(y);
    setAscent(ascent);

    setPos(x,y); //Hier Position festlegen *BUGFIX Position in treeView und saveMap*

    QRectF rec = boundingRect();
    topLeft = rec.topLeft();
    topRight = rec.topRight();
    bottomLeft = rec.bottomLeft();
    bottomRight = rec.bottomRight();
}
/*! Erstellt ein Begrenzungsrechteck für das Tile,Dieses wird sowohl zum zeichnen, als auch für weitere Interaktion benötigt
 * \return Begrenzungsrechteck für das Tile */
QRectF straight::boundingRect() const
{
    return QRectF(0,0,50,50);
}
/*! Zeichnet das Tile
 * \param painter Painter der zum Zeichnen benutzt wird
 * \param option Optionen für das Zeichnen
 * \param widget Widget in welches gezeichnet wird */
void straight::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Hightligh when Selected
    if(selected){
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(3);
        painter->setPen(pen);
        painter->drawRect(boundingRect());
    }

    QPen mainPen;
    mainPen.setColor(Qt::black);
    mainPen.setWidth(2);
    painter->setPen(mainPen);

    painter->drawLine(topLeft, topRight);
    painter->drawLine(bottomLeft,bottomRight);
}

/*! Rotiert das Tile um 90 Grad */
void straight::rotate()
{
    direction++;
    if(direction==2){
        direction = 0;
    }
    QPointF temp = topLeft;
    topLeft = topRight;
    topRight = bottomRight;
    bottomRight = bottomLeft;
    bottomLeft = temp;

    update();
}
/*! Gibt den Typen des Tile zurück
 * \return Typ des Tile */
QString straight::getType()
{
    return "straight";
}

