#ifndef TILE_H
#define TILE_H

#include "point.h"
#include <QString>
#include <QGraphicsItem>
#include <QPainter>
#include <Qdebug>

class Tile : public QGraphicsItem {

protected:
    Point* position;
    double ascent;

    QPointF topLeft;
    QPointF topRight;
    QPointF bottomRight;
    QPointF bottomLeft;
    QPointF topCenter;
    QPointF bottomCenter;
    QPointF leftCenter;
    QPointF rightCenter;

public:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    double getAscent();

    virtual void drawTile();
    virtual QString getType();

    void setAscent(double Ascent);

    Point* getPosition();

    void rotate();

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    Tile();
    Tile(double x, double y, double ascent);
};

#endif
