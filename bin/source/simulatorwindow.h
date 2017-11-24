#ifndef SIMULATORWINDOW_H
#define SIMULATORWINDOW_H
#include "map.h"
#include <QMainWindow>

namespace Ui {
class SimulatorWindow;
}

class SimulatorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimulatorWindow(const Map &nm, QWidget *parent = 0);
    ~SimulatorWindow();

private:
    Map m;
    QGraphicsScene *scene;
    Ui::SimulatorWindow *ui;
    QPainterPath mapPath;
};

#endif // SIMULATORWINDOW_H
