#include "turn.h"

turn::turn()
{
}

/*! Erstellt eine Kurve
 * \param x x-Position der Kurve
 * \param y y-Position der Kurve
 * \param ascent Steigung der Kurve */
turn::turn(double nx, double ny, double nascent, int ndirection)
{
    setAscent(nascent);
    setDirection(ndirection);

    setPos(nx,ny); //Hier Position festlegen *BUGFIX Position in treeView und saveMap*
}
/*! Erstellt ein Begrenzungsrechteck für das Tile,Dieses wird sowohl zum zeichnen, als auch für weitere Interaktion benötigt */
QRectF turn::boundingRect() const
{
    return QRectF(0,0,100,100);
}

/*! Zeichnet das Tile
 * \param painter Painter der zum Zeichnen benutzt wird
 * \param option Optionen für das Zeichnen
 * \param widget Widget in welches gezeichnet wird */
void turn::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

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
    mainPen.setWidth(2);
    painter->setPen(mainPen);

    QRectF rec = boundingRect();
    QBrush brush(Qt::red);

    QPainterPath newPath;

    if (direction == 0) {
        newPath.moveTo(rec.bottomRight());
        newPath.arcTo(rec,0,90);
        newPath.lineTo(rec.topLeft());
        newPath.moveTo(rec.center().x()-50, rec.center().y());
        newPath.lineTo(rec.center());
        newPath.lineTo(rec.center().x(), rec.center().y()+50);
    }
    else if (direction == 1) {
        newPath.moveTo(rec.center().x()+50, rec.center().y());
        newPath.arcTo(rec,0,-90);
        newPath.moveTo(rec.center().x()+50, rec.center().y());
        newPath.lineTo(rec.topRight());
        newPath.moveTo(rec.center());
        newPath.lineTo(rec.center().x(), rec.center().y()-50);
        newPath.moveTo(rec.center());
        newPath.lineTo(rec.center().x()-50, rec.center().y());
        newPath.moveTo(rec.center().x(), rec.center().y()+50);
        newPath.lineTo(rec.bottomLeft());
    }
    else if(direction == 2) {
        newPath.moveTo(rec.center().x()-50, rec.center().y());
        newPath.arcTo(rec,-180,90);
        newPath.lineTo(rec.bottomRight());
        newPath.moveTo(rec.center().x()-50, rec.center().y());
        newPath.lineTo(rec.topLeft());
        newPath.moveTo(rec.center().x(), rec.center().y()-50);
        newPath.lineTo(rec.center());
        newPath.lineTo(rec.center().x()+50, rec.center().y());
        newPath.moveTo(rec.center().x(), rec.center().y()+50);
    }
    else if(direction == 3) {
        newPath.moveTo(rec.bottomLeft());
        newPath.arcTo(rec,-180,-90);
        newPath.lineTo(rec.topRight());
        newPath.moveTo(rec.center().x()+50, rec.center().y());
        newPath.lineTo(rec.center());
        newPath.lineTo(rec.center().x(), rec.center().y()+50);
    }
    this->path = newPath;

    painter->drawPath(this->path);
}

void turn::rotate()
{
    direction++;
    if(direction == 4)
        direction = 0;
    update();
}

/*! Gibt den Typen des Tile zurück
 * \return Typ des Tile */
QString turn::getType()
{
    return "turn";
}

