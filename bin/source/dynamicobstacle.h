#ifndef DYNAMICOBSTACLE_H
#define DYNAMICOBSTACLE_H


#include "obstacle.h"


class  DynamicObstacle: public Obstacle {

    public:

        void accelerate(double acc = 0.1);
        void brake(double br = 0.1);

        double getSpeed();
        void setSpeed(double s);

        int getDirection();
        void setDirection(int d);

        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QString getType();

        DynamicObstacle();
        DynamicObstacle(double x, double y, double width, double length, double speed);
    protected:

        double speed;
        int direction = 0;

};
#endif // DYNAMICOBSTACLE_H
