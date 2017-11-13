#include "tile.h"

Tile::Tile() {
    position = new Point();
    setFlag(QGraphicsItem::ItemIsMovable);
    pressed = false;
    selected = false;
}

Tile::Tile(double x, double y, double Ascent) : ascent(Ascent) {
    position = new Point(x,y);
    setFlag(QGraphicsItem::ItemIsMovable);
    pressed = false;
    selected = false;
    //setPos(x,y);
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

void Tile::setPosition(double x, double y)
{
    position->setX(x);
    position->setY(y);
    //setPos(x,y);
}

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
    if(selected){
       selected = false;
    } else {
       selected = true;
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

