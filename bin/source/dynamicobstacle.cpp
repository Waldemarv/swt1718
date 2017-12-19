#include "dynamicobstacle.h"

DynamicObstacle::DynamicObstacle():Obstacle(),speed(0) {}


DynamicObstacle::DynamicObstacle(double x, double y, double width, double length, double speed, QPointF nstartingPoint, QPointF nendingPoint):
    Obstacle(x,y,width,length),speed(speed) {

    setPos(x,y);
    startingPoint = nstartingPoint;
    endingPoint = nendingPoint;

    endingEllipse = new QGraphicsEllipseItem(QRectF(0,0,10,10), this);
    qreal len = sqrt(((pos().y()-endingPoint.y())*(pos().y()-endingPoint.y())) + (pos().x()-endingPoint.x())*(pos().x()-endingPoint.x()));
    endingEllipse->setPos(len,0);
    endingEllipse->setFlags(ItemIsMovable);
}

void DynamicObstacle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setSelected(false);
    QGraphicsItem::mouseReleaseEvent(event);
}

void DynamicObstacle::advance(int phase)
{
    if(!phase)
        return;
    if((pos().x()-endingPoint.x() < 1) && (pos().y()-endingPoint.y() < 1))
        setPos(startingPoint);
    setPos(mapToParent(speed, 0));
}

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

double DynamicObstacle::calculateRotation()
{
    /* TODO ÜBERARBEITEN FÜR SIMULATION
    QLineF xAxis;
    QLineF movingVector;

    xAxis.setPoints(startingPoint, QPointF(startingPoint.x() + 1000, startingPoint.y()));
    movingVector.setPoints(startingPoint, endingPoint);

    angle = xAxis.angleTo(movingVector);

    if(angle<0)
        angle = angle*(-1);
    return angle;
    */
}

QRectF DynamicObstacle::boundingRect() const{
    return QRectF(0,0,10,10);
}

void DynamicObstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF rec = boundingRect();

    //Startpunkt neu setzen wenn die position geändert wurde
    if(isSelected())
        startingPoint = pos();
    endingPoint = endingEllipse->scenePos();
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

    painter->drawLine(QPointF(0,0), endingEllipse->pos());
}

QString DynamicObstacle::getType(){
    return "dynamicObstacle";
}


void DynamicObstacle::setStartingPoint(QPointF s)
{
    startingPoint = s;
}

void DynamicObstacle::setEndingPoint(QPointF e)
{
    endingPoint = e;
}

QPointF DynamicObstacle::getStartingPoint()
{
    return startingPoint;
}

QPointF DynamicObstacle::getEndingPoint()
{
    return endingPoint;
}

