#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "point.h"
#include <QString>
#include <QGraphicsItem>
#include <QPainter>
#include <Qdebug>

class Obstacle :public QGraphicsItem {

private:
    Point* position;
	double width;
	double length;

public:
    double getwidth();
    double getlength();

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString getType();

    Obstacle();
    Obstacle(double x, double y, double width, double length);

    Point* getPosition();
};

#endif
