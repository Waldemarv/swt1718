#include "obstacle.h"

/*! Gibt den Typ des Obstacle zurück
 * \return Typ des Obstacle */
QString Obstacle::getType()
{
    return "staticObstacle";
}

QPointF Obstacle::getStartingPoint()
{

}

QPointF Obstacle::getEndingPoint()
{

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
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //Nur zum Testen
    QRectF rec = boundingRect();

    //Hightligh when Selected
    if(isSelected()){
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

double Obstacle::getSpeed()
{
    return 0;
}

void Obstacle::setSpeed(double s)
{
    Q_UNUSED(s);
}

bool Obstacle::isClicked()
{
    return clicked;
}

void Obstacle::setClicked(bool c)
{
    clicked = c;
}
/*! Gibt die Breite des Obstacle zurück
 * \return Breite des Obstacle */
double Obstacle::getwidth() {return width; }

/*! Gibt die Position des Obstacle zurück
 * \return Position des Obstacle */
QPointF* Obstacle::getPosition() { return position; }

/*! Erstellt ein neues Obstacle ohne Größe und Position auf der Map */
Obstacle::Obstacle()
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    clicked = false;
}

/*! Erstellt ein neues Obstacle
 * \param x x-Koordinate des Obstacle
 * \param y y-Koordinate des Obstacle
 * \param width Breite des Obstacle
 * \param length Höhe des Obstacle */
Obstacle::Obstacle(double x, double y, double width, double length) : width(width), length(length)
{
    setPos(x,y);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    clicked = false;
}

/*! Verändert die Position des Obstacle
 * \param x neue x-Koordinate des Obstacle
 * \param y neue y-Koordinate des Obstacle */
void Obstacle::setPosition(double x, double y)
{
    setPos(x,y);
}

double Obstacle::calculateRotation()
{
    return 0;
}

// Mouse Event Handlers
/*! Wählt ein Obstacle an, wenn es angeklickt wurde */
void Obstacle::advance(int phase)
{
    Q_UNUSED(phase);
}

void Obstacle::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
        }
    }

    else if(event->button() == Qt::LeftButton)
        clicked = true;

    update();
    QGraphicsItem::mousePressEvent(event);
}

void Obstacle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
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

