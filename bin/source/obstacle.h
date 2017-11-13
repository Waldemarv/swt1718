#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "point.h"
#include <QString>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

class Obstacle :public QGraphicsItem {

private:
    Point* position;
	double width;
	double length;

public:
    bool pressed;
    bool selected;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);  //TODO: Für löschen augewählter Elemente sowie "Auswahlumrandung" implementieren
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    double getwidth();
    double getlength();

    void updatePosition();
    void setPosition(double x, double y);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString getType();

    Obstacle();
    Obstacle(double x, double y, double width, double length);

    Point* getPosition();
};

#endif
