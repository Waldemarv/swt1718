#include "simulatorcmdl.h"

SimulatorCMDL::SimulatorCMDL(Map &nm, QObject *parent) : QObject(parent)
{
    if(&nm == nullptr) {
        //TODO : Egal was geklickt wird : Es muss eine Map geladen werden
    }
    else {

        this->m = nm;
        //Scene erstellen
        scene = new QGraphicsScene(this);
        //Größe der Scene setzen
        scene->setSceneRect(0,0,m.getSizeX()*m.getGridSize(),m.getSizeY()*m.getGridSize());
        //Gesamtpfad der Map bestimmen und zeichnen
        this->mapPath = m.getMapPath();
        scene->addPath(mapPath);
        mapBoundaries.setPath(m.getMapPath());

        sv=0;

        // Timer erstellen
        frontTimer = new QTimer();
        leftTimer = new QTimer();
        rightTimer = new QTimer();
        collisionDetectionTimer = new QTimer();
        sensorsTimer = new QTimer();

        //Verbinde die Timer mit der fortbewegung(advance) und auslenkung(left), (right), sowie der collisionDeteciton
        connect(frontTimer, SIGNAL(timeout()), scene, SLOT(advance()));
        connect(leftTimer, &QTimer::timeout, [this]{ sv->left(); });
        connect(rightTimer, &QTimer::timeout, [this]{ sv->right(); });
        connect(collisionDetectionTimer, &QTimer::timeout, [this] { collisionDetection(); });

        startSimulation();
    }
}

// Für Simulator umändern
void SimulatorCMDL::collisionDetection() {

    /*verhalten für Andere Vehicles nutzen*/
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

    //Kollision vom SmartVehicle
    if(!mapBoundaries.collidesWithItem(sv,mode)) {
    }
    else {
        //Kollision
        qDebug()<<"collision at: "<<sv->pos();
        collisionDetectionTimer->stop();
        frontTimer->stop();
        leftTimer->stop();
        rightTimer->stop();
        sensorsTimer->stop();
        //startSimulation();

        //Abfrage ob das Smart Vehicle am Endpunkt kollidiert ist
        /*if((int)m.getEndingTile()->pos().x() > (int)sv->pos().x())
        {
            if((int)m.getEndingTile()->pos().y() > (int)sv->pos().y())
            {
                if((int)m.getEndingTile()->pos().y() % (int)sv->pos().y() < 50 && (int)m.getEndingTile()->pos().x() % (int)sv->pos().x() < 50)
                    qDebug()<<"Ziel Erreicht";
                else
                    qDebug()<<"Ziel nicht Erreicht";
            }
            else
            {
                if((int)sv->pos().y() % (int)m.getEndingTile()->pos().y() < 50 && (int)m.getEndingTile()->pos().x() % (int)sv->pos().x() < 50)
                    qDebug()<<"Ziel Erreicht";
                else
                    qDebug()<<"Ziel nicht Erreicht";
            }
        }
        else
        {
            if((int)m.getEndingTile()->pos().y() > (int)sv->pos().y())
            {
                if((int)m.getEndingTile()->pos().y() % (int)sv->pos().y() < 50 && (int)sv->pos().x() % (int)m.getEndingTile()->pos().x() < 50)
                    qDebug()<<"Ziel Erreicht";
                else
                    qDebug()<<"Ziel nicht Erreicht";
            }
            else
            {
                if((int)sv->pos().y() % (int)m.getEndingTile()->pos().y() < 50 && (int)sv->pos().x() % (int)m.getEndingTile()->pos().x() < 50)
                    qDebug()<<"Ziel Erreicht";
                else
                    qDebug()<<"Ziel nicht Erreicht";
            }
        }*/

    }
}

void SimulatorCMDL::startSimulation()
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

    //Automatische Abstandserkennung der Senoren
    /*connect(sensorsTimer, &QTimer::timeout, [this] {

    for(int i=0; i<sv->getNumberOfSensors(); i++)
    {
        int length;
        for(int j=0;j<1000; j=j+10)
        {
            sv->getSensor(i)->setLength(j);
            if(mapBoundaries.collidesWithItem(sv->getSensor(i), mode))
            {
                for(int k=0; k<10; k++)
                {
                    if(!mapBoundaries.collidesWithItem(sv->getSensor(i), mode))
                    {
                        length = j-k;
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
        qDebug()<<"Senor: "<<i<<" , length: "<<length;
    }
    });

    sensorsTimer->start(100);*/
}
