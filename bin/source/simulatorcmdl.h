#ifndef SIMULATORCMDL_H
#define SIMULATORCMDL_H

#include "map.h"
#include "math.h"
#include <QTimer>
#include <QTime>
#include <QString>
#include <QObject>

class SimulatorCMDL : public QObject
{
    Q_OBJECT
public:
    explicit SimulatorCMDL(Map &nm, QObject *parent = nullptr);

    void startSimulation();

    void collisionDetection();

private:
    Map m;
    QGraphicsScene *scene;
    QPainterPath mapPath;
    SmartVehicle* sv;
    QTimer *frontTimer;
    QTimer *leftTimer;
    QTimer *rightTimer;
    QTimer *collisionDetectionTimer;
    QTimer *sensorsTimer;

    QGraphicsPathItem mapBoundaries;
    Qt::ItemSelectionMode mode = Qt::IntersectsItemShape;

    QTime fitnessTime;


signals:

public slots:
};

#endif // SIMULATORCMDL_H
