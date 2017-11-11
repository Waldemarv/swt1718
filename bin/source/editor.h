#ifndef EDITOR_H
#define EDITOR_H

#include "map.h"
#include "ui_editor.h"

#include <QtCore>
#include <QGraphicsItem>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QtXml>

#include <QMainWindow>

namespace Ui {
class Editor;
}

class Editor : public QMainWindow
{
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = 0);
    ~Editor();

    void createMap();
    void saveMap();
    void loadMap();



private slots:
    void on_actionNeue_Map_triggered();

    void on_actionSchliessen_triggered();

    void on_actionMap_Speichern_triggered();

    void on_actionMap_Laden_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Editor *ui;
    Map* m;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rect;
};

#endif // EDITOR_H
