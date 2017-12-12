#include "endingtile.h"


Endingtile::Endingtile()
{
}

Endingtile::Endingtile(int nx, int ny, double nascent, int ndirection)
{
    setPos(nx,ny);
    setAscent(nascent);
    setDirection(ndirection);
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
Endingtile *Endingtile::createEndingTile(int nx, int ny, double nascent, int ndirection)
{
    if(endingTilePointer == 0)
    {
        endingTilePointer = new Endingtile(nx,ny,nascent, ndirection);
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
    if(isSelected()){
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

    QRectF rec = boundingRect();

    QPainterPath newPath;
    if (direction == 0)
    {
        newPath.moveTo(rec.topRight());
        newPath.lineTo(rec.topLeft());
        newPath.lineTo(rec.bottomLeft());
        newPath.lineTo(rec.bottomRight());
        newPath.moveTo(rec.topRight());

    }
    else if (direction == 1)
    {
        newPath.moveTo(rec.bottomLeft());
        newPath.lineTo(rec.topLeft());
        newPath.lineTo(rec.topRight());
        newPath.lineTo(rec.bottomRight());
        newPath.moveTo(rec.bottomLeft());

    }
    else if(direction == 2)
    {
        newPath.moveTo(rec.topLeft());
        newPath.lineTo(rec.topRight());
        newPath.lineTo(rec.bottomRight());
        newPath.lineTo(rec.bottomLeft());
        newPath.moveTo(rec.topLeft());

    }
    else if (direction == 3)
    {
        newPath.moveTo(rec.topLeft());
        newPath.lineTo(rec.bottomLeft());
        newPath.lineTo(rec.bottomRight());
        newPath.lineTo(rec.topRight());
        newPath.moveTo(rec.topLeft());

    }
    this->path=newPath;
    painter->drawPath(path);
}

/*! Rotiert das Tile */
void Endingtile::rotate()
{
    direction++;
    if(direction==4){
        direction = 0;
    }
    update();
}


Endingtile* Endingtile::endingTilePointer = 0;
