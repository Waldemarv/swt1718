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
    setAscent(nascent);
    setDirection(ndirection);
    setPos(nx,ny); //Hier Position festlegen *BUGFIX Position in treeView und saveMap*
}

/*! Erstellt ein Begrenzungsrechteck für das Tile,Dieses wird sowohl zum zeichnen, als auch für weitere Interaktion benötigt */
QRectF Tintersection::boundingRect() const
{ //Unterschiedliches BoundingRect je nach Ausrichtung des Tiles
    if(this->direction%4 == 0 || this->direction%4==2) {
    return QRectF(0,0,150,100);
    }
    else {
    return QRectF(0,0,100,150);
    }
}

/*! Zeichnet das Tile
 * \param painter Painter der zum Zeichnen benutzt wird
 * \param option Optionen für das Zeichnen
 * \param widget Widget in welches gezeichnet wird */
void Tintersection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

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
    mainPen.setWidth(2);
    painter->setPen(mainPen);
    QBrush brush(Qt::red);
    QPainterPath newPath;

    QRectF rec = boundingRect();

    //Für jede Ausrichtung muss das Tile neu gezeichnet werden.
    if(direction%4 == 0) {
        newPath.moveTo(rec.topLeft());
        newPath.lineTo(rec.topRight());
        newPath.moveTo(rec.bottomRight().x()-50,rec.bottomRight().y());
        newPath.lineTo(rec.bottomRight().x()-50,rec.bottomRight().y()-50);
        newPath.lineTo(rec.bottomRight().x(),rec.bottomRight().y()-50);
        newPath.moveTo(rec.bottomLeft().x()+50,rec.bottomLeft().y());
        newPath.lineTo(rec.bottomLeft().x()+50,rec.bottomLeft().y()-50);
        newPath.lineTo(rec.bottomLeft().x(),rec.bottomLeft().y()-50);
        direction = 0;
    }
    else if(direction == 1) {
        newPath.moveTo(rec.topRight());
        newPath.lineTo(rec.bottomRight());
        newPath.moveTo(rec.topLeft().x(),rec.topLeft().y()+50);
        newPath.lineTo(rec.topLeft().x()+50,rec.topLeft().y()+50);
        newPath.lineTo(rec.topLeft().x()+50,rec.topLeft().y());
        newPath.moveTo(rec.topLeft().x(),rec.topLeft().y()+100);
        newPath.lineTo(rec.topLeft().x()+50,rec.topLeft().y()+100);
        newPath.lineTo(rec.topLeft().x()+50,rec.topLeft().y()+150);
    }
    else if(direction == 2) {
        newPath.moveTo(rec.bottomLeft());
        newPath.lineTo(rec.bottomRight());
        newPath.moveTo(rec.topLeft().x()+50,rec.topLeft().y());
        newPath.lineTo(rec.topLeft().x()+50,rec.topLeft().y()+50);
        newPath.lineTo(rec.topLeft().x(),rec.topLeft().y()+50);
        newPath.moveTo(rec.topLeft().x()+100,rec.topLeft().y());
        newPath.lineTo(rec.topLeft().x()+100,rec.topLeft().y()+50);
        newPath.lineTo(rec.topLeft().x()+150,rec.topLeft().y()+50);
    }
    else if(direction == 3) {
        newPath.moveTo(rec.topLeft());
        newPath.lineTo(rec.bottomLeft());
        newPath.moveTo(rec.topLeft().x()+50,rec.topLeft().y());
        newPath.lineTo(rec.topLeft().x()+50,rec.topLeft().y()+50);
        newPath.lineTo(rec.topLeft().x()+100,rec.topLeft().y()+50);
        newPath.moveTo(rec.topLeft().x()+50,rec.topLeft().y()+150);
        newPath.lineTo(rec.topLeft().x()+50,rec.topLeft().y()+100);
        newPath.lineTo(rec.topLeft().x()+100,rec.topLeft().y()+100);
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
