#ifndef SIMULATORWINDOW_H
#define SIMULATORWINDOW_H
#include "map.h"
#include "math.h"
#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>

namespace Ui {
class SimulatorWindow;
}

class SimulatorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimulatorWindow(const Map &nm, QWidget *parent = 0);
    ~SimulatorWindow();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    double getLeftDist();

private slots:
    void on_actionSimulation_starten_triggered();
    void collisionDetection();

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
    bool simulationStarted;
};

#endif // SIMULATORWINDOW_H
