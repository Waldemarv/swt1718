#include "straight.h"

/*! Erstellt ein leeres gerades Streckenstück */
straight::straight()
{
}

/*! Erstellt ein gerades Streckenstück
 * \param x x-Koordinate der Geraden
 * \param y y-Koordinate der Geraden
 * \param ascent Steigung der Geraden */
straight::straight(double nx, double ny, double nascent, int ndirection)
{
    setAscent(nascent);
    setDirection(ndirection);

    setPos(nx,ny); //Hier Position festlegen *BUGFIX Position in treeView und saveMap*
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
    if(isSelected()){
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(3);
        painter->setPen(pen);
        painter->drawRect(boundingRect());
    }

    QRectF rec = boundingRect();
    QPen mainPen;
    mainPen.setColor(Qt::black);
    mainPen.setWidth(2);
    painter->setPen(mainPen);

    QPainterPath newPath;
    if(direction == 0)
    {
        newPath.moveTo(rec.topLeft());
        newPath.lineTo(rec.topRight());
        newPath.moveTo(rec.bottomLeft());
        newPath.lineTo(rec.bottomRight());
    }
    else if(direction == 1)
    {
        newPath.moveTo(rec.topLeft());
        newPath.lineTo(rec.bottomLeft());
        newPath.moveTo(rec.topRight());
        newPath.lineTo(rec.bottomRight());
    }

    this->path = newPath;

    painter->drawPath(this->path);
}

/*! Rotiert das Tile um 90 Grad */
void straight::rotate()
{
    direction++;
    if(direction==2){
        direction = 0;
    }
    update();
}
/*! Gibt den Typen des Tile zurück
 * \return Typ des Tile */
QString straight::getType()
{
    return "straight";
}

