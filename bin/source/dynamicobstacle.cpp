#include "dynamicobstacle.h"

DynamicObstacle::DynamicObstacle():Obstacle(),speed(0) {}

/*! Erstellt ein dynamic Obstacle
 * \param x Position in x-Richtung des Obstacle
 * \param y Position in y-Richtung des Obstacle
 * \param rotation Rotation des Obstacle
 * \param width Breite des Obstacle
 * \param length Länge des Obstacle
 * \param speed Geschwindigkeit des Obstacle
 * \param nstartingPoint Startpunkt des Obstacle
 * \param nendingPoint Endpunkt des Obstacle
 * */
DynamicObstacle::DynamicObstacle(double x, double y, double rotation, double width, double length, double speed, QPointF nstartingPoint, QPointF nendingPoint):
    Obstacle(x,y,width,length),speed(speed), angle(rotation) {

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
/*! Funktion für die Bewegung des dynamic Obstacle*/
void DynamicObstacle::advance(int phase)
{
    if(!phase)
        return;
    if((pos().x()-endingPoint.x() < 1) && (pos().y()-endingPoint.y() < 1))
        setPos(startingPoint);
    setPos(mapToParent(speed, 0));
}

/*! Funktion für die Beschleunigung des Obstacle
 * \param acc Wert, um wie viel das Obstacle beschleunigt werden soll */
void DynamicObstacle::accelerate(double acc){
    speed+=acc;
}
/*! Funktion für das Abbremsen des Obstacle
 * \param br Wert, um wie viel das Obstacle gebremst werden soll*/
void DynamicObstacle::brake(double br){
    if(speed > 0){
        speed-=br;
    } else {
        speed = 0;
    }
}

/*! Gibt die aktuelle Geschwindigkeit des Obstacle aus
 * \return Geschwindigkeit des Obstacle */
double DynamicObstacle::getSpeed(){
    return speed;
}
/*! Setzt direkt die Geschwindigkeit des Obstacle
 * \param s Geschwindigkeit des Obstacle */
void DynamicObstacle::setSpeed(double s){
    speed = s;
}

int DynamicObstacle::getDirection(){
    return direction;
}

void DynamicObstacle::setDirection(int d){
    direction = (direction + d)%4 ;
}

/*! Berechnet die aktuelle Rotation des Obstacle
 * \return Rotation des Obstacle */
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
/*! Erstellt ein Begrenzungsrechteck für das Obstacle */
QRectF DynamicObstacle::boundingRect() const{
    return QRectF(0,0,10,10);
}
/*! Zeichnet das Obstacle*/
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

