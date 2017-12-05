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

    double getLeftDist();

private slots:
    void on_actionSimulation_starten_triggered();
    void collisionDetection();

    void on_actionZu_Editor_wechseln_triggered();

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

    QTime fitnessTime;
};

#endif // SIMULATORWINDOW_H
