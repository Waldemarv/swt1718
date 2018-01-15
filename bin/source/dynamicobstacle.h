#ifndef DYNAMICOBSTACLE_H
#define DYNAMICOBSTACLE_H


#include "obstacle.h"
#include "stdio.h"
#include "math.h"
#include <QTimer>
#include <QtCore>

class  DynamicObstacle: public Obstacle {

    public:
        void accelerate(double acc = 0.1);
        void brake(double br = 0.1);

        double getSpeed();
        void setSpeed(double s);

        int getDirection();
        void setDirection(int d);

        double calculateRotation();

        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QString getType();

        void setStartingPoint(QPointF s);
        void setEndingPoint(QPointF e);

        QPointF getStartingPoint();
        QPointF getEndingPoint();

        DynamicObstacle();
        DynamicObstacle(double x, double y, double rotation, double width, double length, double speed, QPointF nstartingPoint, QPointF nendingPoint);

        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    protected:
        double speed;
        double angle;

        int direction = 0;

        void advance(int phase);

        QPointF startingPoint;
        QPointF endingPoint;

        QGraphicsItem *endingEllipse;

};
#endif // DYNAMICOBSTACLE_H
