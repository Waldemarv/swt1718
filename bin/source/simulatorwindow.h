#ifndef SIMULATORWINDOW_H
#define SIMULATORWINDOW_H
#include "map.h"
#include "math.h"
#include "editor.h"
#include "neuralnet.h"
#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QKeyEvent>
#include <QString>
#include <vector>
#include <QtCore>
#include <QtXml>

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

    void saveNet();
    void loadNet();

private slots:
    void on_actionSimulation_starten_triggered();
    void collisionDetection();
    void vehicleControls();

    void on_actionZu_Editor_wechseln_triggered();
    void pauseSimulation();
    void resumeSimulation();

    void connectSensorCalculation();
    void connectFitnessTime();

    void on_actionPause_triggered();

    void on_actionResume_triggered();

    void on_trainingModeButton_clicked();

    void on_autonomousModeButton_clicked();

    void on_trainModelButton_clicked();

    void on_actionsaveNeuralNet_triggered();

    void on_actionloadNeuralNet_triggered();

private:
    Map m;
    QGraphicsScene *scene;
    Ui::SimulatorWindow *ui;
    QPainterPath mapPath;
    SmartVehicle* sv;
    QTimer *straightTimer;
    QTimer *leftTimer;
    QTimer *rightTimer;
    QTimer *mainTimer;
    QTimer *sensorsTimer;

    QGraphicsPathItem mapBoundaries;
    Qt::ItemSelectionMode mode = Qt::IntersectsItemShape;
    QRectF goal;

    QTime fitnessTime;
    int tempTime;
    int pauseTime;

    double left;
    double right;
    double straight;
    bool collision;
    bool trainingMode = true;

    double speed = 0;

    std::vector<std::vector<double>> sensorData;
    std::vector<std::vector<double>> steeringData;

    neuralNet *drivingNet;
    std::vector<unsigned> topology;

};

#endif // SIMULATORWINDOW_H
