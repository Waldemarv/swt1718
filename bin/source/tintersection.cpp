#include "tintersection.h"

Tintersection::Tintersection()
{

}
/*! Erstellt eine T-Kreuzung und fügt sie dem Editor hinzu.
 * \param x x-Koordinate der T-Kreuzung
 * \param y y-Koordinate der T-Kreuzung
 * \param ascent Steigung der T-Kreuzung */
Tintersection::Tintersection(double nx, double ny, double nascent, int ndirection)
{
    position->setX(nx);
    position->setY(ny);
    setAscent(nascent);
    setDirection(ndirection);

    setPos(nx,ny); //Hier Position festlegen *BUGFIX Position in treeView und saveMap*

    QRectF rec = boundingRect();
    topLeft = rec.topLeft();
    topRight = rec.topRight();
    bottomLeft = rec.bottomLeft();
    bottomRight = rec.bottomRight();
    topCenter = QPointF(rec.center().x(), rec.center().y()-50);
    bottomCenter = QPointF(rec.center().x(), rec.center().y()+50);
    leftCenter = QPointF(rec.center().x()-50, rec.center().y());
    rightCenter = QPointF(rec.center().x()+50, rec.center().y());
}

/*! Erstellt ein Begrenzungsrechteck für das Tile,Dieses wird sowohl zum zeichnen, als auch für weitere Interaktion benötigt */
QRectF Tintersection::boundingRect() const
{ //Unterschiedliches BoundingRect je nach Ausrichtung des Tiles
    if(this->direction%4 == 0 || this->direction%4==2) {
    return QRectF(x(),y(),150,100);
    }
    else {
    return QRectF(x(),y(),100,150);
    }
}

/*! Zeichnet das Tile
 * \param painter Painter der zum Zeichnen benutzt wird
 * \param option Optionen für das Zeichnen
 * \param widget Widget in welches gezeichnet wird */
void Tintersection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

    QPainterPath newPath;

    //Für jede Ausrichtung muss das Tile neu gezeichnet werden.
    if(direction%4 == 0) {
        newPath.moveTo(topLeft);
        newPath.lineTo(topRight);
        newPath.moveTo(bottomRight.x()-50,bottomRight.y());
        newPath.lineTo(bottomRight.x()-50,bottomRight.y()-50);
        newPath.lineTo(bottomRight.x(),bottomRight.y()-50);
        newPath.moveTo(bottomLeft.x()+50,bottomLeft.y());
        newPath.lineTo(bottomLeft.x()+50,bottomLeft.y()-50);
        newPath.lineTo(bottomLeft.x(),bottomLeft.y()-50);
        direction = 0;
    }
    else if(direction == 1) {
        QRectF rect = boundingRect();
        newPath.moveTo(rect.topRight());
        newPath.lineTo(rect.bottomRight());
        newPath.moveTo(rect.topLeft().x(),rect.topLeft().y()+50);
        newPath.lineTo(rect.topLeft().x()+50,rect.topLeft().y()+50);
        newPath.lineTo(rect.topLeft().x()+50,rect.topLeft().y());
        newPath.moveTo(rect.topLeft().x(),rect.topLeft().y()+100);
        newPath.lineTo(rect.topLeft().x()+50,rect.topLeft().y()+100);
        newPath.lineTo(rect.topLeft().x()+50,rect.topLeft().y()+150);
    }
    else if(direction == 2) {
        QRectF rect = boundingRect();
        newPath.moveTo(rect.bottomLeft());
        newPath.lineTo(rect.bottomRight());
        newPath.moveTo(rect.topLeft().x()+50,rect.topLeft().y());
        newPath.lineTo(rect.topLeft().x()+50,rect.topLeft().y()+50);
        newPath.lineTo(rect.topLeft().x(),rect.topLeft().y()+50);
        newPath.moveTo(rect.topLeft().x()+100,rect.topLeft().y());
        newPath.lineTo(rect.topLeft().x()+100,rect.topLeft().y()+50);
        newPath.lineTo(rect.topLeft().x()+150,rect.topLeft().y()+50);
    }
    else if(direction == 3) {
        QRectF rect = boundingRect();
        newPath.moveTo(rect.topLeft());
        newPath.lineTo(rect.bottomLeft());
        newPath.moveTo(rect.topLeft().x()+50,rect.topLeft().y());
        newPath.lineTo(rect.topLeft().x()+50,rect.topLeft().y()+50);
        newPath.lineTo(rect.topLeft().x()+100,rect.topLeft().y()+50);
        newPath.moveTo(rect.topLeft().x()+50,rect.topLeft().y()+150);
        newPath.lineTo(rect.topLeft().x()+50,rect.topLeft().y()+100);
        newPath.lineTo(rect.topLeft().x()+100,rect.topLeft().y()+100);
    }

    this->path=newPath;
    painter->drawPath(this->path);
}

/*! Rotiert das Tile */
void Tintersection::rotate()
{
    this->direction++;
    if(direction == 4)
        direction=0;
}


QString Tintersection::getType()
{
    return "T-Intersection";
}
