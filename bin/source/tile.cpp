#include "tile.h"

/*! Erstellt ein Tile im Koordinatenursprung */
Tile::Tile() {
    position = new QPointF();
    setFlag(QGraphicsItem::ItemIsMovable);
    selected = false;
}
/*! Erstellt ein Tile
 * \param x x-Position des Tile
 * \param y y-Position des Tile
 * \param ascent Steigung des Tile */
Tile::Tile(double x, double y, double ascent) : ascent(ascent) {
    position = new QPointF(x,y);
    setFlag(QGraphicsItem::ItemIsMovable);
    selected = false;
}

/*! Erstellt ein Begrenzungsrechteck für das Tile,Dieses wird sowohl zum zeichnen, als auch für weitere Interaktion benötigt
 * \return Begrenzungsrechteck für das Tile */
QRectF Tile::boundingRect()
{
    return QRectF(x(),y(),50,50);
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

int Tile::getDirection()
{
    return direction;
}

void Tile::fitIntoGrid()
{
    /* Im folgenden bin ich von 4 Quadraten ausgegenagen in denen sich das Objekt bewegt, da egal an welcher Stelle der Map, dieser Fall eintritt.
     * Das Objekt bewegt sich also in den Koordinaten x=0,...,100 und y=0,...,100.
     * Mit dem ModuloOperator ist es möglich Die Objekte genau Auf diese 4 Quadranten Abzubilden.
     * Danach wird entschieden welchem Quadranten das Objekt zugeordnet wird und dementsprechend die osition gesetzt. */

    if(((int)x() % 50) < 25)
    {
        if(((int)y() % 50) < 25)
        {
            setPos((x()-((int)x()%50)), (y()-((int)y()%50)));
        }
        else
        {
            setPos(x()-((int)x()%50), y() + (50-((int)y()%50)));
        }
    }
    else
    {
        if(((int)y() % 50) <25)
        {
            setPos(x() + (50 - (int)x()%50), y()-((int)y()%50));
        }
        else
        {
            setPos(x() + (50 - (int)x()%50), y() + (50 - (int)y()%50));
        }
    }
}

/*! Ändert die Position des Tile
 * \param x neue x-Koordinate des Tile
 * \param y neue y-Koordinate des Tile */
void Tile::setPosition(double x, double y)
{
    position->setX(x);
    position->setY(y);
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

void Tile::setDirection(double ndirection)
{
    direction = ndirection;
}

/*! Gibt die Position des Tile zurück
 * \return Position des Tile */
QPointF* Tile::getPosition() { return position; }

/*! Rotiert das Tile um 90 Grad */
void Tile::rotate() {}

/*! Bei Doppelklick wird das Tile rotiert */
void Tile::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    rotate();
    update();
    QGraphicsItem::mouseDoubleClickEvent(event);
}
/*! Wählt das angeklickte Tile aus */
void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(selected){
       selected = false;
    } else {
       selected = true;
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Tile::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    fitIntoGrid();
    QGraphicsItem::mouseReleaseEvent(event);
}

/* Item soll nicht außerhalb der Scene bewegbar sein */
void Tile::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
        QGraphicsItem::mouseMoveEvent(event);

       if (x() < 0)
       {
           setPos(0, y());
       }
       else if (x() + boundingRect().right() > scene()->width())
       {
           setPos(scene()->width() - boundingRect().width(), y());
       }

       if (y() < 0)
       {
           setPos(x(), 0);
       }
       else if ( y()+ boundingRect().bottom() > scene()->height())
       {
           setPos(x(), scene()->height() - boundingRect().height());
       }
}

/*! Gibt den Path des Tile zurück
 * \returns Path des Tiles */
QPainterPath Tile::getPath(){
    return this->path;
}
