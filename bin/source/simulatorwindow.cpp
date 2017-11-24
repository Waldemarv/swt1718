#include "simulatorwindow.h"
#include "ui_simulatorwindow.h"

SimulatorWindow::SimulatorWindow(const Map &nm, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimulatorWindow)
{
    ui->setupUi(this);
    if(&nm == nullptr) {
        //TODO : Egal was geklickt wird : Es muss eine Map geladen werden
        qDebug() << "Leerer Simulator geöffnet" << endl;
    }
    else {
        this->m = nm;
        qDebug() << "Simulator mit Map geöffnet" << endl;
        //Scene erstellen
        scene = new QGraphicsScene(this);
        ui->graphicsView->setScene(scene);
        //Größe der Scene setzen
        scene->setSceneRect(0,0,m.getSizeX()*m.getGridSize(),m.getSizeY()*m.getGridSize());
        ui->graphicsView->setScene(scene);
        qDebug() << "Szene gesetzt";
        //Gesamtpfad der Map bestimmen und zeichnen
        this->mapPath = m.getMapPath();
        scene->addPath(mapPath);
        qDebug() << "Map gezeichnet";
    }
}

// Für Simulator umänderns
/*void Editor::collisionDetection() {
    QGraphicsPathItem mapBoundaries;
    mapBoundaries.setPath(m->getMapPath());

    Qt::ItemSelectionMode mode = Qt::IntersectsItemShape; //Modus für Kollisionsabfrage


    if(mapBoundaries.collidesWithItem(sv,mode)) {
        qDebug() << "Kollision!" << endl;
    }
}
*/
SimulatorWindow::~SimulatorWindow()
{
    delete ui;
}
