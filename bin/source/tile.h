#ifndef TILE_H
#define TILE_H

#include "point.h"
#include <QString>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>

/*! Realisiert Streckenelemente einer Map */
class Tile : public QGraphicsItem  {
protected:
    Point* position;
    double ascent;
    QPainterPath path;
    int direction;

    QPointF topLeft;
    QPointF topRight;
    QPointF bottomRight;
    QPointF bottomLeft;
    QPointF topCenter;
    QPointF bottomCenter;
    QPointF leftCenter;
    QPointF rightCenter;

public:
    Tile();
    Tile(double x, double y, double ascent);

    bool released;
    bool selected;

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString getType();

    double getAscent();
    int getIndex();

    void fitIntoGrid();
    void setIndex(int i);

    void setPosition(double x, double y);
    void setAscent(double Ascent);
    virtual void rotate();
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    Point* getPosition();
};

#endif
