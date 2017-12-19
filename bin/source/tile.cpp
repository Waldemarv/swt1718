#include "tile.h"

/*! Erstellt ein Tile im Koordinatenursprung */
Tile::Tile() {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    clicked = false;
}
/*! Erstellt ein Tile
 * \param x x-Position des Tile
 * \param y y-Position des Tile
 * \param ascent Steigung des Tile */
Tile::Tile(double x, double y, double ascent) : ascent(ascent) {
    position = new QPointF(x,y);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    clicked = false;
}

bool Tile::isClicked()
{
    return clicked;
}

void Tile::setClicked(bool c)
{
    clicked = c;
}

/*! Erstellt ein Begrenzungsrechteck für das Tile,Dieses wird sowohl zum zeichnen, als auch für weitere Interaktion benötigt
 * \return Begrenzungsrechteck für das Tile */
QRectF Tile::boundingRect()
{
    return QRectF(0,0,50,50);
}

/*! Zeichnet das Tile
 * \param painter Painter der zum Zeichnen benutzt wird
 * \param option Optionen für das Zeichnen
 * \param widget Widget in welches gezeichnet wird */
void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

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

void Tile::fitIntoScene()
{
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

void Tile::setPath(QPainterPath p)
{
    this->path = p;
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
    if(event->button() == Qt::RightButton)
    {
        if(isSelected() == true)
        {
            setSelected(false);
        }
        else if(isSelected() == false)
        {
            setSelected(true);
            rightButtonSelect = true;
        }
    }

    else if(event->button() == Qt::LeftButton)
    {
        clicked = true;
        rightButtonSelect = false;
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Tile::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!rightButtonSelect)
    {
        setSelected(false);
        rightButtonSelect = false;
    }

    QGraphicsItem::mouseReleaseEvent(event);
}

/*! Gibt den Path des Tile zurück
 * \returns Path des Tiles */
QPainterPath Tile::getPath(){
    return this->path;
}
