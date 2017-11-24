#include "intersection.h"
Intersection::Intersection(){}

/*! Erstellt eine Kreuzung und fügt diese dem Editor hinzu
 * \param x x-Koordinate der Kreuzung
 * \param y y-Koordinate der Kreuzung
 * \param ascent Steigung der Kreuzung */
Intersection::Intersection(double x, double y , double ascent)
{
    setAscent(ascent);

    setPos(x,y); //Hier Position festlegen *BUGFIX Position in treeView und saveMap*

    QRectF rec = boundingRect();
    topLeft = rec.topLeft();
    topRight = rec.topRight();
    bottomLeft = rec.bottomLeft();
    bottomRight = rec.bottomRight();
    topCenter = QPointF(rec.center().x(), rec.center().y()-75);
    bottomCenter = QPointF(rec.center().x(), rec.center().y()+75);
    leftCenter = QPointF(rec.center().x()-75, rec.center().y());
    rightCenter = QPointF(rec.center().x()+75, rec.center().y());
    direction=0;

    // Pfad für Kreuzung wird gezeichnet
    QPainterPath path;
    path.moveTo(rec.center().x()-25,rec.center().y()-75);
    path.lineTo(rec.center().x()-25,rec.center().y()-25);
    path.lineTo(rec.center().x()-75,rec.center().y()-25);
    path.moveTo(rec.center().x()-75,rec.center().y()+25);
    path.lineTo(rec.center().x()-25,rec.center().y()+25);
    path.lineTo(rec.center().x()-25,rec.center().y()+75);
    path.moveTo(rec.center().x()+25,rec.center().y()+75);
    path.lineTo(rec.center().x()+25,rec.center().y()+25);
    path.lineTo(rec.center().x()+75,rec.center().y()+25);
    path.moveTo(rec.center().x()+75,rec.center().y()-25);
    path.lineTo(rec.center().x()+25,rec.center().y()-25);
    path.lineTo(rec.center().x()+25,rec.center().y()-75);

    this->path = path;

}

/*! Erstellt ein Begrenzungsrechteck für das Tile,Dieses wird sowohl zum zeichnen, als auch für weitere Interaktion benötigt */
QRectF Intersection::boundingRect() const
{
    return QRectF(x(),y(),150,150);
}

/*! Zeichnet das Tile
 * \param painter Painter der zum Zeichnen benutzt wird
 * \param option Optionen für das Zeichnen
 * \param widget Widget in welches gezeichnet wird */
void Intersection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

/*! Gibt den Typen des Tile zurück
 * \return Typ des Tile */
QString Intersection::getType()
{
    return "Intersection";
}
