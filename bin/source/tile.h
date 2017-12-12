#ifndef TILE_H
#define TILE_H

#include "point.h"
#include <QString>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

/*! Realisiert Streckenelemente einer Map */
class Tile : public QGraphicsItem  {

protected:
    QPointF* position;
    double ascent;
    QPainterPath path;
    int direction = 0;

    QPointF topLeft;
    QPointF topRight;
    QPointF bottomRight;
    QPointF bottomLeft;
    QPointF topCenter;
    QPointF bottomCenter;
    QPointF leftCenter;
    QPointF rightCenter;

    bool clicked;

public:
    Tile();
    Tile(double x, double y, double ascent);

    bool isClicked();
    void setClicked(bool c);

    virtual QRectF boundingRect();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString getType();

    double getAscent();
    int getDirection();

    void fitIntoGrid();
    void fitIntoScene();

    void setIndex(int i);

    void setPosition(double x, double y);
    void setAscent(double Ascent);
    void setDirection(double ndirection);
    void setPath(QPainterPath p);

    virtual void rotate();
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    QPainterPath getPath();
    QPointF* getPosition();


};

#endif
