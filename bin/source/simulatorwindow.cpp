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

        mapBoundaries.setPath(m.getMapPath());


        sv=0;
        pauseTime = 0;

        // Timer erstellen
        frontTimer = new QTimer();
        leftTimer = new QTimer();
        rightTimer = new QTimer();
        collisionDetectionTimer = new QTimer();
        sensorsTimer = new QTimer();
        accelerationTimer = new QTimer();
        breakTimer = new QTimer();
        slowTimer = new QTimer();

        //Verbinde die Timer mit der fortbewegung(advance) und auslenkung(left), (right), sowie der collisionDeteciton
        connect(frontTimer, SIGNAL(timeout()), scene, SLOT(advance()));
        connect(leftTimer, &QTimer::timeout, [this]{ sv->left(); });
        connect(rightTimer, &QTimer::timeout, [this]{ sv->right(); });
        connect(collisionDetectionTimer, &QTimer::timeout, [this] { collisionDetection(); });

        //Beschleunigungs- und Bremsphysik
        connect(frontTimer, &QTimer::timeout, [this] { physics(); });

        ui->graphicsView->setEnabled(false);
    }
}

// Für Simulator umänderns
void SimulatorWindow::collisionDetection() {
    //Kein Sensor an
    /*if(!mapBoundaries.collidesWithItem(sv->getSensor(0), mode) && !mapBoundaries.collidesWithItem(sv->getSensor(1), mode) && !mapBoundaries.collidesWithItem(sv->getSensor(2), mode))
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
    }*/

    //Kollision vom SmartVehicle
    if(!mapBoundaries.collidesWithItem(sv,mode)) {
        sv->setColor(Qt::green);
    }
    else {
        //Kollision
        qDebug()<<"collision at: "<<sv->pos();
        sv->setColor(Qt::red);
        collisionDetectionTimer->stop();
        frontTimer->stop();
        leftTimer->stop();
        rightTimer->stop();
        accelerationTimer->stop();
        breakTimer->stop();
        slowTimer->stop();
        speed=0;
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
            slowTimer->stop();
            breakTimer->stop();
            accelerationTimer->start(10);
        }
        else if(event->key() == Qt::Key_Left) {
            leftTimer->start(10);
        }

        else if(event->key() == Qt::Key_Right) {
            rightTimer->start(10);
        }
        else if(event->key() == Qt::Key_Down)
        {
            accelerationTimer->stop();
            slowTimer->stop();
            breakTimer->start(10);
        }


        QWidget::keyPressEvent(event);
}

void SimulatorWindow::keyReleaseEvent(QKeyEvent *event)
{
        //Schalte Timer wieder aus, wenn Knopf losgelassen wird
        if(event->key() == Qt::Key_Up)
        {
            accelerationTimer->stop();
            slowTimer->start(10);
        }
        else if(event->key() == Qt::Key_Left)
            leftTimer->stop();
        else if(event->key() == Qt::Key_Right)
            rightTimer->stop();

        else if(event->key() == Qt::Key_Down)
        {
            accelerationTimer->stop();
            slowTimer->start();
            breakTimer->stop();
        }

        QWidget::keyReleaseEvent(event);

}

void SimulatorWindow::startSimulation()
{
    qDebug()<<"Simulation wurde gestartet!";

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
        accelerationTimer->stop();
        breakTimer->stop();
        slowTimer->stop();
        speed = 0;
    }
    // Autonomes Fahrzeug hinzufügen
    sv = new SmartVehicle(0,0,3,m.getStartingPoint().x(), m.getStartingPoint().y());
    scene->addItem(sv);
    //Sensoren der scene hinzufügen
    for(int i = 0; i<sv->getNumberOfSensors(); i++)
        scene->addItem(sv->getSensor(i));
    //Timer der die collisionsdetection Funktion aufruft starten
    collisionDetectionTimer->start();
    //fitnessTime starten und aktualisieren
    fitnessTime.start();
    connect(collisionDetectionTimer, &QTimer::timeout, [this] {

        QString time = QString("%1:%2.%3").arg(QString::number((fitnessTime.elapsed()-pauseTime)/60000), QString::number((fitnessTime.elapsed()-pauseTime)/1000), QString::number((fitnessTime.elapsed()-pauseTime)%1000));
        ui->timeLabel->setText(time);
    });

    //frontTimer starten um beschleunigungs und bremssignale zu erhalten
    frontTimer->start(10);

    //Automatische Abstandserkennung der Senoren
    connect(sensorsTimer, &QTimer::timeout, [this] {

    for(int i=0; i<sv->getNumberOfSensors(); i++)
    {
        for(int j=0;j<1000; j=j+10)
        {
            sv->getSensor(i)->setLength(j);
            if(mapBoundaries.collidesWithItem(sv->getSensor(i), mode))
            {
                for(int k=0; k<10; k++)
                {
                    if(!mapBoundaries.collidesWithItem(sv->getSensor(i), mode))
                    {
                        qDebug()<<(j-k);
                        break;
                    }
                    else
                    {
                        sv->getSensor(i)->setLength(j-k);
                    }
                }
                break;
            }
        }
        scene->update();
    }
    });

    sensorsTimer->start(10);
}

void SimulatorWindow::physics()
{
    //Beschleunigung
    if(accelerationTimer->isActive())
    {
        if(speed < 2.0)
            speed += 0.1;
        else if(speed >= 2.0 && speed < 3.0)
            speed += 0.01;
        else if(speed >= 3.0 && speed < 4.0)
            speed += 0.001;
    }
    //Motorbremse ohne gas oder bremse
    else if(slowTimer->isActive())
    {
        if(speed > 0)
            speed -= 0.08;
        else if(speed < 0)
            speed = 0;
    }
    //Bremse
    else if(breakTimer->isActive())
    {
        if(speed > 0)
            speed -= 0.5;
        else if(speed < 0)
            speed = 0;
    }

    sv->setSpeed(speed);
}

void SimulatorWindow::on_actionZu_Editor_wechseln_triggered()
{
    this->hide();
    pauseSimulation();
    this->parentWidget()->show();
}

void SimulatorWindow::pauseSimulation()
{
    if(frontTimer->isActive())
        frontTimer->stop();

    if(leftTimer->isActive())
        leftTimer->stop();

    if(rightTimer->isActive())
        rightTimer->stop();

    collisionDetectionTimer->stop();
    sensorsTimer->stop();

    tempTime = fitnessTime.elapsed();

    qDebug()<<"Simulation paused";
}

void SimulatorWindow::resumeSimulation()
{
    pauseTime = pauseTime + fitnessTime.elapsed() - tempTime;

    collisionDetectionTimer->start();
    sensorsTimer->start();

    qDebug()<<"Simulation resumed";
}

void SimulatorWindow::on_actionPause_triggered()
{
    pauseSimulation();
}

void SimulatorWindow::on_actionResume_triggered()
{
    resumeSimulation();
}
