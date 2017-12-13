#ifndef SIMULATORWINDOW_H
#define SIMULATORWINDOW_H
#include "map.h"
#include "math.h"
#include "editor.h"
#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QKeyEvent>
#include <QString>

namespace Ui {
class SimulatorWindow;
}

class SimulatorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimulatorWindow(const Map &nm, QWidget *parent);
    ~SimulatorWindow();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void startSimulation();    
    void physics();

private slots:
    void on_actionSimulation_starten_triggered();
    void collisionDetection();

    void on_actionZu_Editor_wechseln_triggered();
    void pauseSimulation();
    void resumeSimulation();

    void connectSensorCalculation();
    void connectFitnessTime();

    void on_actionPause_triggered();

    void on_actionResume_triggered();

private:
    Map m;
    QGraphicsScene *scene;
    Ui::SimulatorWindow *ui;
    QPainterPath mapPath;
    SmartVehicle* sv;
    QTimer *frontTimer;
    QTimer *leftTimer;
    QTimer *rightTimer;
    QTimer *collisionDetectionTimer;
    QTimer *sensorsTimer;
    QTimer *accelerationTimer;
    QTimer *breakTimer;
    QTimer *slowTimer;

    QGraphicsPathItem mapBoundaries;
    Qt::ItemSelectionMode mode = Qt::IntersectsItemShape;
    QRectF goal;

    QTime fitnessTime;
    int tempTime;
    int pauseTime;

    double speed = 0;
};

#endif // SIMULATORWINDOW_H
