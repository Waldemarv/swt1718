#include "turn.h"

turn::turn()
{
}

/*! Erstellt eine Kurve
 * \param x x-Position der Kurve
 * \param y y-Position der Kurve
 * \param ascent Steigung der Kurve */
turn::turn(double x, double y, double ascent)
{
    position->setX(x);
    position->setY(y);
    setAscent(ascent);

    QRectF rec = boundingRect();
    topLeft = rec.topLeft();
    topRight = rec.topRight();
    bottomLeft = rec.bottomLeft();
    bottomRight = rec.bottomRight();
    topCenter = QPointF(rec.center().x(), rec.center().y()-50);
    bottomCenter = QPointF(rec.center().x(), rec.center().y()+50);
    leftCenter = QPointF(rec.center().x()-50, rec.center().y());
    rightCenter = QPointF(rec.center().x()+50, rec.center().y());
    direction=0;

    // Pfad für Standardausrichtung der Kurve wird gezeichnet.
    QPainterPath path;
    path.moveTo(bottomRight);
    path.arcTo(rec,0,90);
    path.lineTo(topLeft);
    path.moveTo(leftCenter);
    path.lineTo(rec.center());
    path.lineTo(bottomCenter);
    this->path = path;
}
/*! Erstellt ein Begrenzungsrechteck für das Tile,Dieses wird sowohl zum zeichnen, als auch für weitere Interaktion benötigt */
QRectF turn::boundingRect() const
{
    return QRectF(position->getX(),position->getY(),100,100);
}

/*! Zeichnet das Tile
 * \param painter Painter der zum Zeichnen benutzt wird
 * \param option Optionen für das Zeichnen
 * \param widget Widget in welches gezeichnet wird */
void turn::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
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

    painter->drawPath(this->path);
}

/*! Rotiert das Tile */
void turn::rotate()
{
    direction++;
    QRectF rec = boundingRect();
    QPainterPath newPath;

    if (direction%4 == 0) {
        newPath.moveTo(bottomRight);
        newPath.arcTo(rec,0,90);
        newPath.lineTo(topLeft);
        newPath.moveTo(leftCenter);
        newPath.lineTo(rec.center());
        newPath.lineTo(bottomCenter);
    }
    else if (direction% 4 == 1) {
        newPath.moveTo(bottomLeft);
        newPath.arcTo(rec,-180,-90);
        newPath.lineTo(topRight);
        newPath.moveTo(rightCenter);
        newPath.lineTo(rec.center());
        newPath.lineTo(bottomCenter);
    }
    else if(direction%4 == 2) {
        newPath.moveTo(rightCenter);
        newPath.arcTo(rec,0,-90);
        newPath.moveTo(rightCenter);
        newPath.lineTo(topRight);
        newPath.moveTo(rec.center());
        newPath.lineTo(topCenter);
        newPath.moveTo(rec.center());
        newPath.lineTo(leftCenter);
        newPath.moveTo(bottomCenter);
        newPath.lineTo(bottomLeft);
    }
    else if(direction%4 == 3) {
        newPath.moveTo(leftCenter);
        newPath.arcTo(rec,-180,90);
        newPath.lineTo(bottomRight);
        newPath.moveTo(leftCenter);
        newPath.lineTo(topLeft);
        newPath.moveTo(topCenter);
        newPath.lineTo(rec.center());
        newPath.lineTo(rightCenter);
        newPath.moveTo(bottomCenter);

    }
    this->path = newPath;

    update();

}

/*! Gibt den Typen des Tile zurück
 * \return Typ des Tile */
QString turn::getType()
{
    return "turn";
}

