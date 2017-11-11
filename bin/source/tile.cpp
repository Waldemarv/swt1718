#include "tile.h"

Tile::Tile() {
    position = new Point();
    setFlag(QGraphicsItem::ItemIsMovable);
}

Tile::Tile(double x, double y, double Ascent) : ascent(Ascent) {
    position = new Point(x,y);
    setFlag(QGraphicsItem::ItemIsMovable);
}

void Tile::drawTile() {
}

QRectF Tile::boundingRect() const
{
    return QRectF(position->getX(),position->getY(),50,50);
}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
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

void Tile::rotate()
{
    QPointF temp = topLeft;
    topLeft = topRight;
    topRight = bottomRight;
    bottomRight = bottomLeft;
    bottomLeft = temp;

    temp=leftCenter;
    leftCenter = topCenter;
    topCenter = rightCenter;
    rightCenter = bottomCenter;
    bottomCenter = temp;

    update();
}

void Tile::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    rotate();
    QGraphicsItem::mouseDoubleClickEvent(event);
}
