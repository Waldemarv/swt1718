#include "dynamicobstacle.h"

DynamicObstacle::DynamicObstacle():Obstacle(),speed(0) {}


DynamicObstacle::DynamicObstacle(double x, double y, double width, double length, double speed):
    Obstacle(x,y,width,length),speed(speed) {}


void DynamicObstacle::accelerate(double acc){
    speed+=acc;
}

void DynamicObstacle::brake(double br){
    if(speed > 0){
        speed-=br;
    } else {
        speed = 0;
    }
}

double DynamicObstacle::getSpeed(){
    return speed;
}

void DynamicObstacle::setSpeed(double s){
    speed = s;
}

int DynamicObstacle::getDirection(){
return direction;
}

void DynamicObstacle::setDirection(int d){
    direction = (direction + d)%4 ;
}

QRectF DynamicObstacle::boundingRect() const{
    return QRectF(0,0,10,10);
}

void DynamicObstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF rec = boundingRect();

    //Hightligh when Selected
    if(isSelected()){
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(3);
        painter->setPen(pen);
        painter->drawRect(boundingRect());
    }
    // Change Pen: Color to Black and Size to 3
    QPen mainPen;
    mainPen.setColor(Qt::black);
    mainPen.setWidth(1);
    painter->setPen(mainPen);

    QBrush brush(Qt::green);
    painter->fillRect(rec, brush);
    painter->drawRect(rec);
}

QString DynamicObstacle::getType(){
    return "dynamicObstacle";
}

