#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "point.h"
#include <QString>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

/*! Klasse für statische Hindernisse */
class Obstacle :public QGraphicsItem {

private:
    QPointF* position;
    double width;
    double length;
    bool clicked;

public:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);  //TODO: Für löschen augewählter Elemente sowie "Auswahlumrandung" implementieren
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    double getwidth();
    double getlength();

    bool isClicked();
    void setClicked(bool c);

    void updatePosition();
    void setPosition(double x, double y);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString getType();

    Obstacle();
    Obstacle(double x, double y, double width, double length);

    QPointF* getPosition();
};

#endif
