#include "startingtile.h"

Startingtile::Startingtile()
{
}

Startingtile::Startingtile(int nx, int ny, double nascent, int ndirection)
{
    setPos(nx,ny);
    setAscent(nascent);
    setDirection(ndirection);
}

/*! Erstellt den Startabschnitt der Strecke*/
Startingtile::~Startingtile()
{
    startingTilePointer = 0;
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
    return "StartingTile";
}

/*! Prüft ob die Map schon einen Startabschnitt hat, falls nicht, wird einer Erzeugt
\returns Pointer der auf den Startabschnitt zeigt*/
Startingtile *Startingtile::createStartingTile(int nx, int ny, double nascent, int ndireciton)
{
    if(startingTilePointer == 0)
    {
        startingTilePointer = new Startingtile(nx, ny, nascent, ndireciton);
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

    QRectF rec = boundingRect();

    QPainterPath newPath;
    if (direction == 0)
    {
        newPath.moveTo(rec.topLeft());
        newPath.lineTo(rec.topRight());
        newPath.moveTo(rec.bottomLeft());
        newPath.lineTo(rec.bottomRight());
        newPath.moveTo(rec.topLeft());
        newPath.lineTo(rec.bottomLeft());
    }
    else if (direction == 1)
    {
        newPath.moveTo(rec.topLeft());
        newPath.lineTo(rec.bottomLeft());
        newPath.moveTo(rec.topLeft());
        newPath.lineTo(rec.topRight());
        newPath.lineTo(rec.bottomRight());
    }
    else if(direction == 2)
    {
        newPath.moveTo(rec.topLeft());
        newPath.lineTo(rec.topRight());
        newPath.moveTo(rec.bottomLeft());
        newPath.lineTo(rec.bottomRight());
        newPath.lineTo(rec.topRight());
    }
    else if (direction == 3)
    {
        newPath.moveTo(rec.topLeft());
        newPath.lineTo(rec.bottomLeft());
        newPath.lineTo(rec.bottomRight());
        newPath.lineTo(rec.topRight());
    }
    this->path=newPath;
    painter->drawPath(path);
}

/*! Rotiert das Tile */
void Startingtile::rotate()
{
    direction++;
    if(direction==4){
    direction = 0;
    }

    update();
}

Startingtile* Startingtile::startingTilePointer = 0;
