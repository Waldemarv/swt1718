#ifndef EDITOR_H
#define EDITOR_H

#include "map.h"
#include "ui_editor.h"
#include "simulatorwindow.h"
#include "simulatorcmdl.h"
#include "caretaker.h"
#include <QtCore>
#include <QGraphicsItem>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QtXml>
#include <QtGui>
#include <QMainWindow>
#include <vector>
#include <QUndoCommand>


namespace Ui {
/*! Stellt grundlegende Funktionen zum Erstellen und bearbeiten von Maps in einer graphischen Oberfläche zur Verfügung */
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
    void clearTree();
    void deleteMap();
    void drawGridLayout(int x, int y);

    void updateTreeNumberOfTiles();
    void updateTreeNumberOfObstacles();
    void updateTreeMapSize();
    void updateMapValues(int x, int y);

    void addTreeItems();
    void addTreeMap(double x, double y);

    void setTreeTilesPosition(QPointF position, int index);
    void setTreeObstaclesPosition(QPointF position, int index);

    void addChild(QTreeWidgetItem *parent, QString name, int posX, int posY);

    void connectScene();
    void connectTreeWidget();

    void autoSave();

    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_actionNeue_Map_triggered();

    void on_actionSchliessen_triggered();

    void on_actionMap_Speichern_triggered();

    void on_actionMap_Laden_triggered();

    void on_straightButton_clicked();

    void on_turnButton_clicked();

    void on_staticObstacleButton_clicked();

    void on_deleteTileButton_clicked();

    void on_deleteObstacleButton_clicked();

    void on_intersectionButton_clicked();

    void on_tIntersectionButton_clicked();

    void on_startingPointButton_clicked();

    void on_endingPointButton_clicked();

    void on_actionSimulation_starten_triggered();

    void on_actionUndo_triggered();

    void on_dynamicObstacleButton_clicked();

private:
    Ui::Editor *ui;
    Map* m;
    QGraphicsScene *scene;
    QTreeWidgetItem* treeRoot;
    QTreeWidgetItem* treeObstacles;
    QTreeWidgetItem* treeTiles;
    QTreeWidgetItem* treePoints;
    Caretaker* caretaker;
    QTimer* autoSaveTimer;
};

#endif // EDITOR_H
