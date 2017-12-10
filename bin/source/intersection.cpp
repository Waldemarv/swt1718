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
}

/*! Erstellt ein Begrenzungsrechteck für das Tile,Dieses wird sowohl zum zeichnen, als auch für weitere Interaktion benötigt */
QRectF Intersection::boundingRect() const
{
    return QRectF(0,0,150,150);
}

/*! Zeichnet das Tile
 * \param painter Painter der zum Zeichnen benutzt wird
 * \param option Optionen für das Zeichnen
 * \param widget Widget in welches gezeichnet wird */
void Intersection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
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

    QRectF rec = boundingRect();
    // Pfad für Kreuzung wird gezeichnet
    QPainterPath newPath;
    newPath.moveTo(rec.center().x()-25,rec.center().y()-75);
    newPath.lineTo(rec.center().x()-25,rec.center().y()-25);
    newPath.lineTo(rec.center().x()-75,rec.center().y()-25);
    newPath.moveTo(rec.center().x()-75,rec.center().y()+25);
    newPath.lineTo(rec.center().x()-25,rec.center().y()+25);
    newPath.lineTo(rec.center().x()-25,rec.center().y()+75);
    newPath.moveTo(rec.center().x()+25,rec.center().y()+75);
    newPath.lineTo(rec.center().x()+25,rec.center().y()+25);
    newPath.lineTo(rec.center().x()+75,rec.center().y()+25);
    newPath.moveTo(rec.center().x()+75,rec.center().y()-25);
    newPath.lineTo(rec.center().x()+25,rec.center().y()-25);
    newPath.lineTo(rec.center().x()+25,rec.center().y()-75);

    this->path = newPath;

    painter->drawPath(this->path);
}

/*! Gibt den Typen des Tile zurück
 * \return Typ des Tile */
QString Intersection::getType()
{
    return "Intersection";
}
