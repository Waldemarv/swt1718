#ifndef EDITOR_H
#define EDITOR_H

#include "map.h"
#include "ui_editor.h"

#include <QGraphicsLayout>
#include <QGraphicsGridLayout>
#include <QtCore>
#include <QGraphicsItem>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QtXml>
#include <QtGui>

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
    void clearTree();
    void deleteMap();

    void updateTreeTiles();
    void updateTreeObstacles();
    void updateSize();

    void addChild(QTreeWidgetItem *parent, QString name, int posX, int posY);

private slots:
    void on_actionNeue_Map_triggered();

    void on_actionSchliessen_triggered();

    void on_actionMap_Speichern_triggered();

    void on_actionMap_Laden_triggered();

    void on_straightButton_clicked();

    void on_turnButton_clicked();

    void on_deleteTile_clicked();

    void on_staticObstacle_clicked();

    void on_deleteObstacle_clicked();

    void on_deleteSelectedTile_clicked();

    void on_deleteSelectedObstacle_clicked();

private:
    Ui::Editor *ui;
    Map* m;
    QGraphicsScene *scene;
    QTreeWidgetItem* treeRoot;
    QTreeWidgetItem* treeObstacles;
    QTreeWidgetItem* treeTiles;
};

#endif // EDITOR_H
