#include "endingtile.h"


Endingtile::Endingtile()
{
    QRectF rec = boundingRect();
    topLeft = rec.topLeft();
    topRight = rec.topRight();
    bottomLeft = rec.bottomLeft();
    bottomRight = rec.bottomRight();
}
/*! Erstellt den Endabschnitt der Strecke*/
Endingtile::~Endingtile()
{
    endingTilePointer = 0;
}

/*! Erstellt ein Begrenzungsrechteck für das Tile,Dieses wird sowohl zum zeichnen, als auch für weitere Interaktion benötigt */
QRectF Endingtile::boundingRect() const
{
    return QRectF(0,0,50,50);
}

/*! Gibt den Typen des Tile zurück
* \return Typ des Tile */
QString Endingtile::getType()
{
    return "EndingTile";
}

/*! Prüft ob die Map schon einen Endabschnitt hat, falls nicht, wird einer Erzeugt
*\returns Pointer der auf den Endabschnitt zeigt*/
Endingtile *Endingtile::createEndingTile()
{
    if(endingTilePointer == 0)
    {
        endingTilePointer = new Endingtile();
    }
    return endingTilePointer;
}

/*! Zeichnet das Tile
* \param painter Painter der zum Zeichnen benutzt wird
* \param option Optionen für das Zeichnen
* \param widget Widget in welches gezeichnet wird */
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

/*! Rotiert das Tile */
void Endingtile::rotate()
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


Endingtile* Endingtile::endingTilePointer = 0;
