#include "simulatorwindow.h"
#include "ui_simulatorwindow.h"

SimulatorWindow::SimulatorWindow(const Map &nm, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimulatorWindow)
{
    ui->setupUi(this);
    if(&nm == nullptr) {
        //TODO : Egal was geklickt wird : Es muss eine Map geladen werden
    }
    else {
        this->m = nm;
        //Scene erstellen
        scene = new QGraphicsScene(this);
        //Größe der Scene setzen
        scene->setSceneRect(0,0,m.getSizeX()*m.getGridSize(),m.getSizeY()*m.getGridSize());
        ui->graphicsView->setScene(scene);
        //Gesamtpfad der Map bestimmen und zeichnen
        this->mapPath = m.getMapPath();
        scene->addPath(mapPath);

        sv=0;

        // Timer erstellen
        frontTimer = new QTimer();
        leftTimer = new QTimer();
        rightTimer = new QTimer();
        collisionDetectionTimer = new QTimer();

        //Verbinde die Timer mit der fortbewegung(advance) und auslenkung(left), (right), sowie der collisionDeteciton
        connect(frontTimer, SIGNAL(timeout()), scene, SLOT(advance()));
        connect(leftTimer, &QTimer::timeout, [this]{ sv->left(); });
        connect(rightTimer, &QTimer::timeout, [this]{ sv->right(); });
        connect(collisionDetectionTimer, &QTimer::timeout, [this] { collisionDetection(); });

        ui->graphicsView->setEnabled(false);
    }
}

// Für Simulator umänderns
void SimulatorWindow::collisionDetection() {
    QGraphicsPathItem mapBoundaries;
    mapBoundaries.setPath(m.getMapPath());

    Qt::ItemSelectionMode mode = Qt::IntersectsItemShape; //Modus für Kollisionsabfrage
    //Kein Sensor an
    if(!mapBoundaries.collidesWithItem(sv->getSensor(0), mode) && !mapBoundaries.collidesWithItem(sv->getSensor(1), mode) && !mapBoundaries.collidesWithItem(sv->getSensor(2), mode))
    {
        if(leftTimer->isActive())
            leftTimer->stop();
        if(rightTimer->isActive())
            rightTimer->stop();
        if(!frontTimer->isActive())
            frontTimer->start(10);
    }
    //Alle Sensoren an
    else if(mapBoundaries.collidesWithItem(sv->getSensor(0), mode) && mapBoundaries.collidesWithItem(sv->getSensor(1), mode) && mapBoundaries.collidesWithItem(sv->getSensor(2), mode))
    {
        if(leftTimer->isActive())
            leftTimer->stop();
        if(rightTimer->isActive())
            rightTimer->stop();
        if(!frontTimer->isActive())
            frontTimer->start(10);
    }
    //Rechter und linker Sensor an
    else if(mapBoundaries.collidesWithItem(sv->getSensor(0), mode) && mapBoundaries.collidesWithItem(sv->getSensor(1), mode))
    {
        if(leftTimer->isActive())
            leftTimer->stop();
        if(rightTimer->isActive())
            rightTimer->stop();
        if(!frontTimer->isActive())
            frontTimer->start(10);
    }
    //Rechter und mittlerer Sensor an
    else if(mapBoundaries.collidesWithItem(sv->getSensor(0), mode) && mapBoundaries.collidesWithItem(sv->getSensor(2), mode))
    {
        if(!leftTimer->isActive())
            leftTimer->start(10);
        if(rightTimer->isActive())
            rightTimer->stop();
        if(frontTimer->isActive())
            frontTimer->stop();
    }
    //Linker und mittlerer Sensor an
    else if(mapBoundaries.collidesWithItem(sv->getSensor(1), mode) && mapBoundaries.collidesWithItem(sv->getSensor(2), mode))
    {
        if(!rightTimer->isActive())
            rightTimer->start(10);
        if(leftTimer->isActive())
            leftTimer->stop();
        if(frontTimer->isActive())
            frontTimer->stop();
    }
    else if(mapBoundaries.collidesWithItem(sv->getSensor(0), mode))
    {
        //Right Sensor
        if(!leftTimer->isActive())
           leftTimer->start(10);

        if(rightTimer->isActive())
           rightTimer->stop();

        if(!frontTimer->isActive())
            frontTimer->start(10);
    }
    else if (mapBoundaries.collidesWithItem(sv->getSensor(1), mode))
    {
        //Left Sensor
        if(!rightTimer->isActive())
           rightTimer->start(10);

        if(leftTimer->isActive())
           leftTimer->stop();

        if(!frontTimer->isActive())
            frontTimer->start(10);
    }
    else if (mapBoundaries.collidesWithItem(sv->getSensor(1), mode))
    {
        //Front Sensor
        if(!rightTimer->isActive())
           rightTimer->start(10);

        if(leftTimer->isActive())
           leftTimer->stop();

        if(frontTimer->isActive())
            frontTimer->stop();
    }

    for(int i=0; i<sv->getNumberOfSensors(); i++)
    {
        if(!mapBoundaries.collidesWithItem(sv->getSensor(i), mode))
            sv->getSensor(i)->setColor(Qt::green);
        else
           sv->getSensor(i)->setColor(Qt::red);
    }

    //Kollision vom SmartVehicle
    if(!mapBoundaries.collidesWithItem(sv,mode)) {
        sv->setColor(Qt::green);
    }
    else {
        //Kollision
        sv->setColor(Qt::red);
        collisionDetectionTimer->stop();
        frontTimer->stop();
        leftTimer->stop();
        rightTimer->stop();
        //startSimulation();

        //If KollisionPoint == EndingPoint
        /*if((int)m.getEndingTile()->pos().x() > (int)sv->pos().x())
        {
            if((int)m.getEndingTile()->pos().y() > (int)sv->pos().y())
            {
                if((int)m.getEndingTile()->pos().y() % (int)sv->pos().y() < 25 && (int)m.getEndingTile()->pos().x() % (int)sv->pos().x() < 25)
                    QMessageBox::about(this, "Gewonnen", "Herzlichen Glückwunsch !");
            }
            else
            {
                if((int)sv->pos().y() % (int)m.getEndingTile()->pos().y() < 25 && (int)m.getEndingTile()->pos().x() % (int)sv->pos().x() < 25)
                    QMessageBox::about(this, "Gewonnen", "Herzlichen Glückwunsch !");
            }
        }
        else
        {
            if((int)m.getEndingTile()->pos().y() > (int)sv->pos().y())
            {
                if((int)m.getEndingTile()->pos().y() % (int)sv->pos().y() < 25 && (int)sv->pos().x() % (int)m.getEndingTile()->pos().x() < 25)
                    QMessageBox::about(this, "Gewonnen", "Herzlichen Glückwunsch !");
            }
            else
            {
                if((int)sv->pos().y() % (int)m.getEndingTile()->pos().y() < 25 && (int)sv->pos().x() % (int)m.getEndingTile()->pos().x() < 25)
                    QMessageBox::about(this, "Gewonnen", "Herzlichen Glückwunsch !");
            }
        }*/
    }
}

SimulatorWindow::~SimulatorWindow()
{
    delete ui;
}

void SimulatorWindow::on_actionSimulation_starten_triggered()
{
    startSimulation();
}
void SimulatorWindow::keyPressEvent(QKeyEvent *event)
{
        //Starte die Timer wenn bestimmter Knopf gedrückt wird(Nur zu tetszwecken)
        if(event->key() == Qt::Key_Up) {
            frontTimer->start(10);
        }
        else if(event->key() == Qt::Key_Left) {
            leftTimer->start(10);
        }

        else if(event->key() == Qt::Key_Right) {
            rightTimer->start(10);
        }

        QWidget::keyPressEvent(event);
}

void SimulatorWindow::keyReleaseEvent(QKeyEvent *event)
{
        //Schalte Timer wieder aus, wenn Knopf losgelassen wird
        if(event->key() == Qt::Key_Up)
            frontTimer->stop();
        else if(event->key() == Qt::Key_Left)
            leftTimer->stop();
        else if(event->key() == Qt::Key_Right)
            rightTimer->stop();

        QWidget::keyReleaseEvent(event);

}

void SimulatorWindow::startSimulation()
{
    if(sv != 0)
    {
        for(int i = 0; i<sv->getNumberOfSensors(); i++)
        {
            scene->removeItem(sv->getSensor(i));
            delete sv->getSensor(i);
        }
        delete sv;
        frontTimer->stop();
        leftTimer->stop();
        rightTimer->stop();
    }
    // Autonomes Fahrzeug hinzufügen
    sv = new SmartVehicle(0,1,2,m.getStartingPoint().x(), m.getStartingPoint().y());
    scene->addItem(sv);

    for(int i = 0; i<sv->getNumberOfSensors(); i++)
        scene->addItem(sv->getSensor(i));

    collisionDetectionTimer->start();

    fitnessTime.start();

    connect(collisionDetectionTimer, &QTimer::timeout, [this] {

        QString time = QString("%1:%2.%3").arg(QString::number(fitnessTime.elapsed()/60000), QString::number(fitnessTime.elapsed()/1000), QString::number(fitnessTime.elapsed()%1000));
        ui->timeLabel->setText(time);
    });
}
