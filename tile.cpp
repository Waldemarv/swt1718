#include "tile.h"

/*! Erstellt ein Tile im Koordinatenursprung */
Tile::Tile() {
    position = new Point();
    setFlag(QGraphicsItem::ItemIsMovable);
    pressed = false;
    selected = false;

}
/*! Erstellt ein Tile
 * \param x x-Position des Tile
 * \param y y-Position des Tile
 * \param ascent Steigung des Tile */
Tile::Tile(double x, double y, double ascent) : ascent(ascent) {
    position = new Point(x,y);
    setFlag(QGraphicsItem::ItemIsMovable);
    pressed = false;
    selected = false;
    //setPos(x,y);
}

/*! Erstellt ein Begrenzungsrechteck für das Tile,Dieses wird sowohl zum zeichnen, als auch für weitere Interaktion benötigt
 * \return Begrenzungsrechteck für das Tile */
QRectF Tile::boundingRect() const
{
    return QRectF(position->getX(),position->getY(),50,50);
}

/*! Zeichnet das Tile
 * \param painter Painter der zum Zeichnen benutzt wird
 * \param option Optionen für das Zeichnen
 * \param widget Widget in welches gezeichnet wird */
void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Nur zum Testen
    QRectF rec = boundingRect();
    QBrush brush(Qt::green);
    painter->fillRect(rec, brush);
    painter->drawRect(rec);
}

/*! Gibt die Steigung des Tile zurück
 * \return Steigung des Tile */
double Tile::getAscent(){ return ascent; }

/*! Ändert die Position des Tile
 * \param x neue x-Koordinate des Tile
 * \param y neue y-Koordinate des Tile */
void Tile::setPosition(double x, double y)
{
    position->setX(x);
    position->setY(y);
    //setPos(x,y);
}

/*! Gibt den Typen des Tile zurück
 * \return Typ des Tile */
QString Tile::getType()
{
    return "tile";
}

/*! Ändert die Steigung des Tile
 * \param nAscent neue Steigung des Tile */
void Tile::setAscent(double nAscent)
{
    ascent = nAscent;
}

/*! Gibt die Position des Tile zurück
 * \return Position des Tile */
Point* Tile::getPosition() { return position; }

/*! Rotiert das Tile */
void Tile::rotate() {}

/*! Bei Doppelklick wird das Tile rotiert */
void Tile::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    rotate();
    QGraphicsItem::mouseDoubleClickEvent(event);
}
/*! Wählt das angeklickte Tile aus */
void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed=true;
    if(selected){
       selected = false;
    } else {
       selected = true;
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

