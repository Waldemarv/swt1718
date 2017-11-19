#include "startingtile.h"

/*! Erstellt den Startabschnitt der Strecke*/
Startingtile::Startingtile()
{
    QRectF rec = boundingRect();
    topLeft = rec.topLeft();
    topRight = rec.topRight();
    bottomLeft = rec.bottomLeft();
    bottomRight = rec.bottomRight();
}

/*! Erstellt ein Begrenzungsrechteck für das Tile,Dieses wird sowohl zum zeichnen, als auch für weitere Interaktion benötigt */
QRectF Startingtile::boundingRect() const
{
    return QRectF(0,0,50,50);
}

/*! Gibt den Typen des Tile zurück
 * \return Typ des Tile */
QString Startingtile::getType()
{
    return "StartingPoint";
}

/*! Prüft ob die Map schon einen Endabschnitt hat, falls nicht, wird einer Erzeugt
    \returns Pointer der auf den Endabschnitt zeigt*/
Startingtile *Startingtile::createStartingTile()
{
    if(startingTilePointer == 0)
    {
        startingTilePointer = new Startingtile();
    }
    return startingTilePointer;
}

/*! Zeichnet das Tile
 * \param painter Painter der zum Zeichnen benutzt wird
 * \param option Optionen für das Zeichnen
 * \param widget Widget in welches gezeichnet wird */
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

/*! Rotiert das Tile */
void Startingtile::rotate()
{
    direction++;
    if(direction==4){
        direction = 0;
    }

    QPointF temp = topLeft;
    topLeft = topRight;
    topRight = bottomRight;
    bottomRight = bottomLeft;
    bottomLeft = temp;

    update();
}

Startingtile* Startingtile::startingTilePointer = 0;
