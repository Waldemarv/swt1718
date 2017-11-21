#include "obstacle.h"

/*! Gibt den Typ des Obstacle zurück
 * \return Typ des Obstacle */
QString Obstacle::getType()
{
    return "staticObstacle";
}

/*! Erstellt ein Begrenzungsrechteck für das Obstacle. Dieses wird sowohl zum zeichnen, als auch für weitere Interaktion benötigt
 * \return Begrenzungsrechteck des Obstacle */
QRectF Obstacle::boundingRect() const
{
    return QRectF(0,0,width,length);
}

/*! Zeichnet das Obstacle
 * \param painter Painter der zum zeichnen benutzt wird
 * \param option Optionen für das Zeichnen
 * \param widget Widget in welches gezeichnet wird */
void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Nur zum Testen
    QRectF rec = boundingRect();

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
    mainPen.setWidth(1);
    painter->setPen(mainPen);

    QBrush brush(Qt::blue);
    painter->fillRect(rec, brush);
    painter->drawRect(rec);
}

/*! Gibt die Länge des Obstacle zurück
 * \return Länge des Obstacle */
double Obstacle::getlength() { return length; }
/*! Gibt die Breite des Obstacle zurück
 * \return Breite des Obstacle */
double Obstacle::getwidth() {return width; }

/*! Gibt die Position des Obstacle zurück
 * \return Position des Obstacle */
Point* Obstacle::getPosition() { return position; }

/*! Erstellt ein neues Obstacle ohne Größe und Position auf der Map */
Obstacle::Obstacle(){
    position = new Point();
    setFlag(QGraphicsItem::ItemIsMovable);
    released = true;
    selected = false;

}

/*! Erstellt ein neues Obstacle
 * \param x x-Koordinate des Obstacle
 * \param y y-Koordinate des Obstacle
 * \param width Breite des Obstacle
 * \param length Höhe des Obstacle */
Obstacle::Obstacle(double x, double y, double width, double length) : width(width), length(length)
{
    position = new Point(x,y);
    setPos(x,y); //Hier Position festlegen *BUGFIX Position in treeView und saveMap*
    setFlag(QGraphicsItem::ItemIsMovable);
    released = true;
    selected = false;
}

/*! Verändert die Position des Obstacle
 * \param x neue x-Koordinate des Obstacle
 * \param y neue y-Koordinate des Obstacle */
void Obstacle::setPosition(double x, double y)
{
    position->setX(x);
    position->setY(y);
    //setPos(x,y);
}

// Mouse Event Handlers
/*! Wählt ein Obstacle an, wenn es angeklickt wurde */
void Obstacle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    released=false;

    if(selected){
       selected = false;
    } else {
       selected = true;
    }

    update();
    QGraphicsItem::mousePressEvent(event);
}

void Obstacle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    released=true;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Obstacle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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

