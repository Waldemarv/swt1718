#include "simulatorwindow.h"
#include "ui_simulatorwindow.h"

/*! Erstellt ein Simulationsfenster mit Hilfe einer Map, die aus dem Editor geladen wird*/
SimulatorWindow::SimulatorWindow(const Map &nm, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimulatorWindow)
{
    ui->setupUi(this);
    if(&nm == nullptr) 
	{
    }
    else 
	{
        this->m = nm;
        //Scene erstellen
        scene = new QGraphicsScene(this);
        //Größe der Scene setzen
        scene->setSceneRect(0,0,m.getSizeX()*m.getGridSize(),m.getSizeY()*m.getGridSize());
        ui->graphicsView->setScene(scene);
        //Gesamtpfad der Map bestimmen und zeichnen
        QPainterPathStroker stroke;
        this->mapPath = stroke.createStroke(m.getMapPath());
        scene->addPath(mapPath);

        //Rectangle für Ziel erstellen
        goal = QRectF(m.getEndingTile()->pos(),QSizeF(50,50));
        scene->addRect(goal,QPen(Qt::yellow),QBrush(Qt::BDiagPattern));
        //Map Path setzen
        mapBoundaries.setPath(mapPath);
        //Obstacles einfügen
        for(int i=0; i<this->m.getNumberOfObstacles();i++)
        {
            m.getObstacle(i)->setSelected(false);
            if(m.getObstacle(i)->getType() == "dynamicObstacle")
            {
                m.getObstacle(i)->setMode(1); //mode simulation = 1
                m.getObstacle(i)->setRotation(m.getObstacle(i)->calculateRotation());
                m.getObstacle(i)->updateEndingPoint();
            }
            scene->addItem(m.getObstacle(i));
        }

        sv=0;
        pauseTime = 0;
        straight = 0.0;
        left = 0.0;
        right = 0.0;
        drivingNet = 0;
        collision = false;
        ui->actionPause->setEnabled(false);
        ui->actionResume->setEnabled(false);
        ui->actionSimulation_Stoppen->setEnabled(false);


        // Timer erstellen
        straightTimer = new QTimer();
        leftTimer = new QTimer();
        rightTimer = new QTimer();
        mainTimer = new QTimer();
        sensorsTimer = new QTimer();
        dynamicObstacleTimer = new QTimer();

        //Verbinde die Timer mit der fortbewegung(advance) und auslenkung(left), (right), sowie der collisionDeteciton
        connect(mainTimer, SIGNAL(timeout()), scene, SLOT(advance()));
        connect(leftTimer, &QTimer::timeout, [this]{ sv->left(); });
        connect(rightTimer, &QTimer::timeout, [this]{ sv->right(); });
        connect(mainTimer, &QTimer::timeout, [this] { collisionDetection(); vehicleControls(); });
        //Funktionen für firnesstime connect und sensor(Length)Calculation die mit entsprechenden Timern aktiviert werden
        connectFitnessTime();
        connectSensorCalculation();

        ui->graphicsView->setEnabled(true);

        if(trainingMode)
        {
            ui->trainingModeButton->setEnabled(false);
            ui->autonomousModeButton->setEnabled(true);
        }
        else
        {
            ui->autonomousModeButton->setEnabled(false);
            ui->trainingModeButton->setEnabled(true);
        }
    }
}

/*! Funktion für die Kollisionsdetektion. */
void SimulatorWindow::collisionDetection() 
{
    //Keine Kollision mit der Strecke
    if(!mapBoundaries.collidesWithItem(sv,mode)) {
        //Überprüfen ob Ziel erreicht
        if(goal.contains(sv->pos())) {
            qDebug() << "Ziel erreicht!";
            sv->setColor(Qt::blue); 
            collision = false;
            stopSimulation();
        }
        else {
            sv->setColor(Qt::green);
            for(int i=0; i<m.getNumberOfObstacles(); i++)
            {
                if(m.getObstacle(i)->collidesWithItem(sv, mode))
                {
                    //Kollision mit Obstacle
                    qDebug()<<"collision with obstacle at: "<<sv->pos();           
                    sv->setColor(Qt::red);
                    collision = true;
                    stopSimulation();
                }
            }
        }
    }
    else {
        //Kollision mit der Strecke
        qDebug()<<"collision with map at: "<<sv->pos();
        sv->setColor(Qt::red);
        collision = true;

        stopSimulation();
    }
}

/*! Definiert die Steuerung des autonomen Fahrzeugs wichtig für steuerung vom neuronalen Netz */
void SimulatorWindow::vehicleControls()
{
    if(straight)
    {
        sv->setSpeed(1);
        //if(!straightTimer->isActive())
        //    straightTimer->start(10);
    }
    else
    {
        sv->setSpeed(0);
        //if(straightTimer->isActive())
        //    straightTimer->stop();
    }

    if(left)
    {
        if(!leftTimer->isActive())
            leftTimer->start(10);
    }
    else
    {
        leftTimer->stop();
    }

    if(right)
    {
        if(!rightTimer->isActive())
            rightTimer->start(10);
    }
    else
    {
        rightTimer->stop();
    }
}

SimulatorWindow::~SimulatorWindow()
{
    delete ui;
}
/*! Startet die Simulation */
void SimulatorWindow::on_actionSimulation_starten_triggered()
{
    startSimulation();
}

void SimulatorWindow::keyPressEvent(QKeyEvent *event)
{
        //Starte die Timer wenn bestimmter Knopf gedrückt wird(Nur zu tetszwecken)
        if(event->key() == Qt::Key_Up)
            straight = 1.0;
        else if(event->key() == Qt::Key_Left)
            left = 1.0;
        else if(event->key() == Qt::Key_Right)
            right = 1.0;

        QWidget::keyPressEvent(event);
}

void SimulatorWindow::keyReleaseEvent(QKeyEvent *event)
{
        //Schalte Timer wieder aus, wenn Knopf losgelassen wird
        if(event->key() == Qt::Key_Up)
            straight = 0.0;
        else if(event->key() == Qt::Key_Left)
            left = 0.0;
        else if(event->key() == Qt::Key_Right)
            right = 0.0;

        QWidget::keyReleaseEvent(event);

}

/*! Startet die Simulation der geladenen Strecke */
void SimulatorWindow::startSimulation()
{ 
    if(drivingNet == 0)
    {
        QMessageBox::warning(this, "Kein neuronales Netz vorhanden!", "Bitte erstellen oder laden Sie vor der Simulation ein neuonales Netz.");
        return;
    }

    ui->graphicsView->setEnabled(false);
    ui->actionSimulation_Stoppen->setEnabled(true);
    ui->trainModelButton->setEnabled(false);
    ui->autonomousModeButton->setEnabled(false);
    ui->trainingModeButton->setEnabled(false);
    ui->graphicsView->setFocus();
    ui->actionPause->setEnabled(true);
    ui->actionSimulation_Stoppen->setEnabled(true);

    if(sv != 0)
    {
        sv->setPos(m.getStartingPoint().x(), m.getStartingPoint().y());
        sv->setRotation(svStartRotation);
        sv->setAngle(svStartRotation);
        sv->resetSensors();
        straightTimer->stop();
        leftTimer->stop();
        rightTimer->stop();
        mainTimer->stop();
        sensorsTimer->stop();
        speed = 0;
    }
    else
    {
        if(m.getStartingTile()->getDirection() == 0)
        {
            svStartRotation = 0;
        }
        else if(m.getStartingTile()->getDirection() == 1)
        {
            svStartRotation = 90;
        }
        else if(m.getStartingTile()->getDirection() == 2)
        {
            svStartRotation = 180;
        }
        else if(m.getStartingTile()->getDirection() == 3)
        {
            svStartRotation = 270;
        }
        // Autonomes Fahrzeug hinzufügen
        sv = new SmartVehicle(svStartRotation,1,2,m.getStartingPoint().x(), m.getStartingPoint().y());
        scene->addItem(sv);
        //Sensoren der scene hinzufügen
        for(int i = 0; i<sv->getNumberOfSensors(); i++)
            scene->addItem(sv->getSensor(i));
        fitnessTime.restart();
        }

        //Timer der die collisionsdetection Funktion aufruft starten
        mainTimer->start(10);
        //fitnessTime starten und aktualisieren
        fitnessTime.restart();
        //Sensorcalculation aktivieren
        sensorsTimer->start(10);
        //Parameter für kollisionsAbfrage aus falsch setzen damit SV bewegbar und wieder auf Ausgangsvariablen
        collision = false;

        qDebug()<<"Simulation wurde gestartet!";
}

/*! Stoppt die Simulation. Wird entweder durch Input des Benutzers, oder bei Kollision des Fahrzeugs ausgelöst */
void SimulatorWindow::stopSimulation(){
    if(trainingMode)
    {
        ui->trainingModeButton->setEnabled(false);
        ui->autonomousModeButton->setEnabled(true);
    }
    else
    {
        ui->autonomousModeButton->setEnabled(false);
        ui->trainingModeButton->setEnabled(true);
    }

    ui->actionSimulation_starten->setEnabled(true);
    ui->actionSimulation_Stoppen->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->trainModelButton->setEnabled(true);

    straightTimer->stop();
    leftTimer->stop();
    rightTimer->stop();
    speed=0;
    right = left = straight= 0.0;
    mainTimer->stop();
    sensorsTimer->stop();
    collision = false;
}

/*! Speichert das neuronale Netz */
void SimulatorWindow::saveNet()
{

    QString currentPath = QDir::currentPath();
    currentPath.append("/source/Networks/neuralNet.xml");

    QString filename = QFileDialog::getSaveFileName(this, tr("Datei wählen..."),
                                                    currentPath,
                                                    tr("Extensible Markup Language Files (*.xml)"),0,QFileDialog::DontUseNativeDialog);

    //Document erstellen
    QDomDocument document;

    if(filename != NULL)
    {
        QFile file(filename);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::information(this,tr("Datei kann nicht gewählt werden."),file.errorString());
            return ;
        }
        else
        {
            //root festlegen
            QDomElement root = document.createElement("Net");

            //root dem document hinzufügen
            document.appendChild(root);

            QDomElement top = document.createElement("Topology");
            int numberOfLayers = drivingNet->get_m_layers().size() - 2;
            top.setAttribute("Layer-Count", numberOfLayers);

            for(int i=0; i < drivingNet->get_m_layers().size() - 1; i++)  //Last Layer has no weights
            {                
                QDomElement layer = document.createElement("Layer");
                if(i!=0)
                {
                    int neuronsInLayer = drivingNet->get_m_layers()[i].size() - 1;
                    top.setAttribute("Layer"+QString::number(i)+"-Neurons", neuronsInLayer);
                }

                for(int j=0; j < drivingNet->get_m_layers()[i].size(); j++)
                {                    
                    QDomElement neuron = document.createElement("Neuron");
                    neuron.setAttribute("Neuron-Nr", j);

                    for(int k=0; k<drivingNet->get_m_layers()[i][j]->get_m_outputWeights().size() ; k++)
                    {
                        QDomElement weight = document.createElement("Weights");
                        weight.setAttribute("ID", k);
                        weight.setAttribute("weight" ,drivingNet->get_m_layers()[i][j]->get_m_outputWeights()[k].weight);
                        weight.setAttribute("deltaWeight",drivingNet->get_m_layers()[i][j]->get_m_outputWeights()[k].deltaWeight);
                        neuron.appendChild(weight);
                    }
                    layer.appendChild(neuron);
                }
                root.appendChild(layer);
            }
            root.appendChild(top);

            QTextStream stream(&file);
            stream<<document.toString();
        }
    }
}

/*! Lädt ein neuronales Netz vom Rechner */
void SimulatorWindow::loadNet()
{
    //Falls netz vorhanden vorher löschen
    if(drivingNet!=0)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Vor dem Laden speichern..", "Möchten Sie ihr aktuelles neuronales Netz vor dem Laden eines Anderen speichern?\nAndernfalls gehen alle Daten unwiederruflich verloren!",
                                      QMessageBox::Yes|QMessageBox::No|QMessageBox::Abort);

        if (reply == QMessageBox::No) {
            drivingNet = 0;
        }
        if (reply == QMessageBox::Yes) {
            saveNet();
            drivingNet = 0;
            loadNet();
        }
    }
    //Falls topology vorhanden vorher löschen
    if(topology.size()>0)
        topology.clear();


    //Dokument für Datei erstellen
    QDomDocument document;
    //Pfad für die zu ladene Map wählen
    QString currentPath = QDir::currentPath();
    currentPath.append("/source/Networks");


    QString filename = QFileDialog::getOpenFileName(this, tr("Datei wählen..."),
                                                    currentPath,
                                                    tr("Extensible Markup Language Files (*.xml)"),0,QFileDialog::DontUseNativeDialog);


    if(filename != NULL)
    {
        //Ausgewählte Datei öffnen
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::information(this,tr("Datei kann nicht geöffnet werden."),file.errorString());
            return;
        }
        else
        {
            if(!document.setContent(&file))
            {
                QMessageBox::information(this,tr("Dokument kann nicht geladen werden."),file.errorString());
            }
            file.close();
        }

        //Root erstellen
        QDomElement root = document.firstChildElement();

        //Sicherstellen, dass nur Network und keine Map geladen wird
        if(root.tagName() != "Net")
        {
            QMessageBox::warning(this, "Kein Netzwerk..", "Bitte wählen Sie eine Netzwerk datei !");
            return;
        }

        //Liste aller sich im root befindenden Nodes erstellen
        QDomNodeList allLayerNodes = root.childNodes();

        QDomElement top = allLayerNodes.at(allLayerNodes.count()-1).toElement();

        topology.push_back(3);

        for(int a=1; a< top.attribute("Layer-Count").toInt()+1; a++)
        {
            topology.push_back(top.attribute("Layer"+QString::number(a)+"-Neurons").toInt());
        }

        topology.push_back(3);

        createNeuralNet(topology);

        for(int i=0; i< allLayerNodes.count(); i++)
        {
            QDomNodeList allNeuronNodes = allLayerNodes.at(i).childNodes();
            for(int j=0; j<allNeuronNodes.count(); j++)
            {
                QDomNodeList weights = allNeuronNodes.at(j).childNodes();
                for(int k=0; k<weights.count(); k++)
                {
                    double weight = weights.at(k).toElement().attribute("weight").toDouble();
                    double deltaWeight = weights.at(k).toElement().attribute("deltaWeight").toDouble();
                    drivingNet->get_m_layers()[i][j]->setWeights(k, weight, deltaWeight);
                }
            }
        }
    }
}

/*! Erstellt die Topologie des neuronalen Netzes
    \return Topologie des Netzwerkes als Vektor*/
std::vector<unsigned> SimulatorWindow::createTopology()
{
    topology.clear();
    std::vector<unsigned> tempTopology;
    //3inputs (3 sensors)
    tempTopology.push_back(3);

     bool ok;
     int x = QInputDialog::getInt(this, tr("Struktur des neuronalen Netzes"), tr("Anzahl der Hidden Layer:"), 3, 1, 10, 1, &ok);
     if(ok) { //Wenn OK gedrückt wurde :
         for(int i=0; i<x;i++)
         {
             bool ok1 = false;
             QString layernum = QString("Layer"+QString::number(i+1));
             QString neurons = QString("Anzahl der Neuronen in Layer " +QString::number(i+1)+ " auswählen:");
             int y = QInputDialog::getInt(this, layernum, neurons, 1, 6, 100, 1, &ok1);
             if(ok1)
             {
                 tempTopology.push_back(y);
             }
             else
             {
                 QMessageBox::warning(this, "Vorgang abgebrochen.", "Netz konnte nicht vollständig erstellt werden!");
                 return topology;
             }
         }
         //3 outputs (left straight right)
         tempTopology.push_back(3);
         topology = tempTopology;
         return topology;
     }
    else
    {
         QMessageBox::warning(this, "Vorgang abgebrochen.", "Netz konnte nicht vollständig erstellt werden!");
         return topology;
    }
}

/*! Wechselt vom Simulator wieder in den Editor. */
/* void SimulatorWindow::on_actionZu_Editor_wechseln_triggered()
{
    this->hide();
    pauseSimulation();
    this->parentWidget()->show();
} */

/*! Pausiert die Simulation. Timer und position des Fahrzeuges etc. bleiben hierbei erhalten */
void SimulatorWindow::pauseSimulation()
{
    if(straightTimer->isActive())
        straightTimer->stop();

    if(leftTimer->isActive())
        leftTimer->stop();

    if(rightTimer->isActive())
        rightTimer->stop();

    mainTimer->stop();
    sensorsTimer->stop();

    tempTime = fitnessTime.elapsed();

    ui->actionPause->setEnabled(false);
    if(!ui->actionResume->isEnabled())
        ui->actionResume->setEnabled(true);

    qDebug()<<"Simulation paused";
}

/*! Setzt die Simulation nach der Pausierung wieder fort */
void SimulatorWindow::resumeSimulation()
{
    pauseTime = pauseTime + fitnessTime.elapsed() - tempTime;

    mainTimer->start();
    sensorsTimer->start();

    ui->actionResume->setEnabled(false);
    if(!ui->actionPause->isEnabled())
        ui->actionPause->setEnabled(true);

    qDebug()<<"Simulation resumed";
}

/*! Abstandserkennung der Sensoren zu einem Hindernis */
void SimulatorWindow::connectSensorCalculation()
{
    //Automatische Abstandserkennung der Senoren
    connect(sensorsTimer, &QTimer::timeout, [this] {

    /* Die Idee dieser Abstandserkennungsfunktion besteht darin, dass Jeder Sensor iterativ betrachtet wird, dann die länge immer um eine feste anzahl iterativ erhöht wird
     * und bei jeder iteration geschaut wird, ob der sensor auf etwas (Obstacle oder MapPath) gestoßen ist. Falls dies der Fall ist, wird die Länge auf exakt diese Strecke zum nächsten Obstacle/Rand angepast.
     * Somit ist in den Sensoren immer die aktuelle länge gespeichert und sie lässt sich leicht (nahezu) kontinuierlich ausgeben*/
    for(int i=0; i<sv->getNumberOfSensors(); i++)
    {
        /* Hier wird die Sensorlänge hochgezählt. Seitensensoren, haben im Normalfall einen Wert 25-50, dementsprechend würde hier ein Wert leicht über 50 ausreichen, da aber bekannt sein muss, ob bei einer Abbiegung
           in eine Richtung, vorne eine Sackgasse liegt, muss der Sensorwert höher gelegt werden. Bei einem Sensorwert von 200 hat man sowohl Übersicht über meherere Tiles (bis zu 4 Stück) als auch keine unnötige Performance
           -verschwendung für die Seitensensoren, die meist weit unter 200 liegen werden.*/
        for(int j=0;j<200; j=j+10)
        {
            sv->getSensor(i)->setLength(j);

            /*Da Obstacles bewegbar sind, muss hier eine Zwischenfunktion eingefügt werden, die immer alle Sensoren mit allen Obstacles abgleicht.
             * Hierbei wird jedes Obstacle für den aktuell betrachteten Sensor überprüft. Bei einer Kollision mit dem Sensor wird true, andernfalls false zurückgegeben*/
            auto collisionObstacleDetecition = [this, i]{
                for(int o=0; o<m.getNumberOfObstacles(); o++)
                {
                    if(m.getObstacle(o)->collidesWithItem(sv->getSensor(i), mode))
                        return true;
                }
                return false;    };

            /* Hier wird Abgefragt, ob es eine Kollision mit einem Obstacle gab*/
            bool collision = collisionObstacleDetecition();
            if(collision)
            {
                /* Wenn ja, wird der Abstandswert des Sensors auf die länge angepasst*/
                for(int k=0; k<10; k++)
                {
                    collision = collisionObstacleDetecition();
                    if(!collision)
                    {
                        break;
                    }
                    else
                    {
                        sv->getSensor(i)->setLength(j-k);
                    }
                }
                break;
            }
            /* Hier wird Abgefragt, ob es dann alternativ eine Kollision mit dem MapPath gab*/
            else if(mapBoundaries.collidesWithItem(sv->getSensor(i), mode))
            {
                /* Auch hier wird die Länge angepasst*/
               for(int k=0; k<10; k++)
               {
                   if(!mapBoundaries.collidesWithItem(sv->getSensor(i), mode))
                   {
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



    if(trainingMode)
    {

        /*  NO LIFE TRAINING ANYMORE DUE TO PERFORMANCE ISSUES

        //FEED THE NEURAL NET --- LIFE TRAINING//
        //Sensor data
        std::vector<double> inputVals;

        for(int y=0; y<sv->getNumberOfSensors(); y++)
        {
            inputVals.push_back((double)sv->getSensor(y)->getLength()/(double)1000);
        }

        drivingNet->showVectorVals(": Inputs:", inputVals);
        drivingNet->feedForward(inputVals);

        //GET THE NEURAL NETS RESULTS//

        std::vector<double> resultVals;

        drivingNet->getResults(resultVals);
        drivingNet->showVectorVals("Outputs:", resultVals);

        //TRAIN FOR WHAT THE OUTPUTS SHOULD HAVE BEEN//

        //Steering data
        std::vector<double> targetVals;

        targetVals.push_back(left);
        targetVals.push_back(straight);
        targetVals.push_back(right);


        drivingNet->showVectorVals("Targets:", targetVals);
        //assert(targetVals.size() == topology.back());

        //drivingNet->backProp(targetVals);

        qDebug() << "Net recent average error: "
                        << drivingNet->getRecentAverageError();
        */



        //SAFE SENSOR DATA IN MEMORY ---  TRAINING HAPPENS LATER//
        std::vector<double> tempVectorSensors;

        for(int y=0; y<sv->getNumberOfSensors(); y++)
        {
            tempVectorSensors.push_back((double)sv->getSensor(y)->getLength()/(double)1000);
        }
        sensorData.push_back(tempVectorSensors);

        //Safe SteeringData in Memory//
        std::vector<double> tempVectorSteering;

        tempVectorSteering.push_back(left);
        tempVectorSteering.push_back(straight);
        tempVectorSteering.push_back(right);

        steeringData.push_back(tempVectorSteering);
    }

    else
    {
        //Sensor data
        std::vector<double> inputVals;

        for(int y=0; y<sv->getNumberOfSensors(); y++)
        {
            inputVals.push_back((double)sv->getSensor(y)->getLength()/(double)1000);
        }
        drivingNet->showVectorVals(": Inputs:", inputVals);
        drivingNet->feedForward(inputVals);

        //GET THE NEURAL NETS RESULTS AND APPLY THEM//

        std::vector<double> resultVals;

        drivingNet->getResults(resultVals);
        drivingNet->showVectorVals("Outputs:", resultVals);


        left = resultVals.at(0) < 0.5 ? 0.0 : 1;
        straight = resultVals.at(1) < 0.5 ? 0.0 : 1;
        right = resultVals.at(2) < 0.5 ? 0.0 : 1;
    }
    });
}

void SimulatorWindow::connectFitnessTime()
{
    connect(mainTimer, &QTimer::timeout, [this] {

        QString time = QString("%1:%2.%3").arg(QString::number((fitnessTime.elapsed()-pauseTime)/60000), QString::number(((fitnessTime.elapsed()-pauseTime)/1000) % 60), QString::number((fitnessTime.elapsed()-pauseTime)%1000));
        ui->timeLabel->setText(time);
    });
}

void SimulatorWindow::on_actionPause_triggered()
{
    pauseSimulation();
}

void SimulatorWindow::on_actionResume_triggered()
{
    resumeSimulation();
}

/*! Wechselt in den Trainingmodus. Bei diesem fährt der Benutzer das Fahrzeug und bringt ihm somit das selbstständige Fahren bei */
void SimulatorWindow::on_trainingModeButton_clicked()
{
    if(ui->trainingModeButton->isEnabled())
        ui->trainingModeButton->setEnabled(false);
    if(!ui->autonomousModeButton->isEnabled())
        ui->autonomousModeButton->setEnabled(true);
    trainingMode = true;

    //QMessageBox::about(this, "Trainingsmodus..", "Sie befinden sich im Trainingsmodus.Zum starten drücken sie oben 'Simulation starten'. Alle Bewegungsdaten werden aufgezeichnet. Das neuronale Netz wird während der Fahrt trainiert. Sie haben zusätzlich die möglichkeit das Netz mit den aufgenommenen Daten zu trainieren. Drücken Sie dazu anschließen oben auf den 'trainieren' Button und wählen Sie, wie oft trainiert werden soll. Das netz verbessert sich bei jeder Epoche.");

}

/*! Wechselt in den autonomen Modus. Hier fährt das Auto, mithilfe der gesammelten Daten, selbstständig */
void SimulatorWindow::on_autonomousModeButton_clicked()
{
    if(!ui->trainingModeButton->isEnabled())
        ui->trainingModeButton->setEnabled(true);
    if(ui->autonomousModeButton->isEnabled())
        ui->autonomousModeButton->setEnabled(false);
    trainingMode = false;

    //QMessageBox::about(this, "Trainingsmodus..", "Sie befinden sich nun im autonomen Modus. Drücken Sie einfach Simulation starten und sehen Sie wie das neuronale Netz des Fahrzeugs gelernt hat.");

}

/*! Hier wird das Fahrzeug mithilfe der gesammelten Daten über eine beliebige Anzahl an Epochen trainiert */
void SimulatorWindow::on_trainModelButton_clicked()
{
    if(drivingNet == 0)
    {
        QMessageBox::warning(this, "Kein neuronales Netz vorhanden!", "Bitte erstellen oder laden Sie vor der Simulation ein neuonales Netz.");
        return;
    }
    if(sensorData.size() == 0)
    {
        QMessageBox::warning(this, "Keine Trainingsdaten..", "Bitte sammeln Sie zuerst Trainingsdaten indem Sie die Strecke erfolgreich passieren.");
        return;
    }
    //ITERATIVE TRAINING WITH TRAINING DATA//
    bool ok;
    int x = QInputDialog::getInt(this, tr("Trainingsepochen"), tr("Wie oft soll trainiert werden?"), 10000, 100, INT_MAX, 100, &ok);
    if(ok) { //Wenn OK gedrückt wurde :
        for(int i=0; i<x; i++)
        {
            for(int j=0; j<sensorData.size(); j++)
            {
                drivingNet->feedForward(sensorData[j]);

                std::vector<double> resultVals;

                assert(steeringData[j].size() == topology.back());
                drivingNet->backProp(steeringData[j]);
            }
            if(i%100 == 0)
                qDebug() <<"Epoch " << i << ": Net recent average error: "
                            << drivingNet->getRecentAverageError();
        }

        qDebug()<<"DONE TRAINING!";
    }
}

void SimulatorWindow::on_actionsaveNeuralNet_triggered()
{
    saveNet();
}

void SimulatorWindow::on_actionloadNeuralNet_triggered()
{
    loadNet();
}

/*! Erstellt ein neuronales Netz */
void SimulatorWindow::createNeuralNet(std::vector<unsigned> top)
{
    if(topology.size() > 0)
        drivingNet = new neuralNet(topology);
}

void SimulatorWindow::on_actioncreateNeuralNet_triggered()
{
    if(drivingNet != 0)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Vor dem Erstellen speichern?", "Möchten Sie ihr aktuelles neuronales Netz vor dem erstellen eines Anderen speichern?\nAndernfalls gehen alle Daten unwiederruflich verloren!",
                                      QMessageBox::Yes|QMessageBox::No|QMessageBox::Abort);

        if (reply == QMessageBox::No) {
            drivingNet = 0;
            createNeuralNet(createTopology());
        }
        if (reply == QMessageBox::Yes) {
            saveNet();
            drivingNet = 0;
            createNeuralNet(createTopology());
        }
    }
    else
    {
        createNeuralNet(createTopology());
    }
}

void SimulatorWindow::on_actionSimulation_Stoppen_triggered()
{
    stopSimulation();
}
