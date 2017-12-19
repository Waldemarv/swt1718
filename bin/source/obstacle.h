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

protected:
    QPointF* position;
    double width;
    double length;
    bool clicked;

    virtual void advance(int phase);

public:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    double getwidth();
    double getlength();

    virtual double getSpeed();
    virtual void setSpeed(double s);

    bool isClicked();
    void setClicked(bool c);

    void updatePosition();
    void setPosition(double x, double y);

    virtual double calculateRotation();

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString getType();
    virtual QPointF getStartingPoint();
    virtual QPointF getEndingPoint();


    Obstacle();
    Obstacle(double x, double y, double width, double length);

    QPointF* getPosition();
};

#endif
