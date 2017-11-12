#include "tile.h"

Tile::Tile() {
    position = new Point();
    setFlag(QGraphicsItem::ItemIsMovable);
    pressed = false;
}

Tile::Tile(double x, double y, double Ascent) : ascent(Ascent) {
    position = new Point(x,y);
    setFlag(QGraphicsItem::ItemIsMovable);
    pressed = false;
}

QRectF Tile::boundingRect() const
{
    return QRectF(position->getX(),position->getY(),50,50);
}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Nur zum Testen
    QRectF rec = boundingRect();
    QBrush brush(Qt::green);
    painter->fillRect(rec, brush);
    painter->drawRect(rec);
}

double Tile::getAscent(){ return ascent; }

QString Tile::getType()
{
    return "tile";
}

void Tile::setAscent(double Ascent)
{
    ascent = Ascent;
}

Point* Tile::getPosition() { return position; }

void Tile::rotate() {}

void Tile::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    rotate();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed=true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

