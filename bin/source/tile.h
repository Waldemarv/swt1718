#ifndef TILE_H
#define TILE_H

#include "point.h"
#include <QString>
#include <QGraphicsItem>
#include <QPainter>
#include <Qdebug>
#include <QObject>

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
    bool pressed;   //Für Mouse click event

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString getType();

    double getAscent();

    void setPosition(double x, double y);
    void setAscent(double Ascent);
    virtual void rotate();
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);  //TODO: Für löschen augewählter Elemente sowie "Auswahlumrandung" implementieren

    Point* getPosition();

    Tile();
    Tile(double x, double y, double ascent);
};

#endif
