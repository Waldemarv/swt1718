#include "simulatorwindow.h"
#include "ui_simulatorwindow.h"

SimulatorWindow::SimulatorWindow(const Map &nm, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimulatorWindow)
{
    ui->setupUi(this);
    if(&nm == nullptr) 
	{
        //TODO : Egal was geklickt wird : Es muss eine Map geladen werden
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
            scene->addItem(m.getObstacle(i));
            if(m.getObstacle(i)->getType() == "dynamicObstacle")
                m.getObstacle(i)->setRotation(m.getObstacle(i)->calculateRotation());
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


        // Timer erstellen
        straightTimer = new QTimer();
        leftTimer = new QTimer();
        rightTimer = new QTimer();
        mainTimer = new QTimer();
        sensorsTimer = new QTimer();

        //Verbinde die Timer mit der fortbewegung(advance) und auslenkung(left), (right), sowie der collisionDeteciton
        connect(straightTimer, SIGNAL(timeout()), scene, SLOT(advance()));
        connect(leftTimer, &QTimer::timeout, [this]{ sv->left(); });
        connect(rightTimer, &QTimer::timeout, [this]{ sv->right(); });
        connect(mainTimer, &QTimer::timeout, [this] { collisionDetection(); vehicleControls(); });
        //Funktionen für firnesstime connect und sensor(Length)Calculation die mit entsprechenden Timern aktiviert werden
        connectFitnessTime();
        connectSensorCalculation();

        ui->graphicsView->setEnabled(false);

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

void SimulatorWindow::collisionDetection() 
{
    //Keine Kollision mit der Strecke
    if(!mapBoundaries.collidesWithItem(sv,mode)) {
        //Überprüfen ob Ziel erreicht
        if(goal.contains(sv->pos())) {
            qDebug() << "Ziel erreicht!";
            sv->setColor(Qt::blue); 

            straightTimer->stop();
            leftTimer->stop();
            rightTimer->stop();
            speed=0;
            right = left = straight= 0.0;
            mainTimer->stop();
            sensorsTimer->stop();
            collision = false;
        }
        else {
            sv->setColor(Qt::green);
            for(int i=0; i<m.getNumberOfObstacles(); i++)
            {
                if(m.getObstacle(i)->collidesWithItem(sv, mode))
                {
                    //Kollision mit Obstacle
                    qDebug()<<"collision with obstacle at: "<<sv->pos();
                    straightTimer->stop();
                    leftTimer->stop();
                    rightTimer->stop();
                    speed=0;
                    right = left = straight=0.0;
                    mainTimer->stop();
                    sensorsTimer->stop();
                    collision = true;

                    sv->setColor(Qt::red);
                }
            }
        }
    }
    else {
        //Kollision mit der Strecke
        qDebug()<<"collision with map at: "<<sv->pos();
        straightTimer->stop();
        leftTimer->stop();
        rightTimer->stop();
        speed=0;
        right = left = straight=0.0;
        mainTimer->stop();
        sensorsTimer->stop();
        collision = true;

        sv->setColor(Qt::red);
    }
}

void SimulatorWindow::vehicleControls()
{
    if(straight)
    {
        if(!straightTimer->isActive())
            straightTimer->start(10);
    }
    else
    {
        if(straightTimer->isActive())
            straightTimer->stop();
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

void SimulatorWindow::startSimulation()
{
    if(drivingNet == 0)
    {
        QMessageBox::about(this, "Kein neuronales Netz vorhanden!", "Bitte erstellen oder laden Sie vor der Simulation ein neuonales Netz.");
        return;
    }

    qDebug()<<"Simulation wurde gestartet!";

    ui->actionPause->setEnabled(true);

    if(sv != 0)
    {
        sv->setPos(m.getStartingPoint().x(), m.getStartingPoint().y());
        sv->setRotation(0);
        sv->setAngle(0);
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
    // Autonomes Fahrzeug hinzufügen
    sv = new SmartVehicle(0,1,2,m.getStartingPoint().x(), m.getStartingPoint().y());
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
}

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

std::vector<unsigned> SimulatorWindow::createTopology()
{
    std::vector<unsigned> tempTopology;
     //3inputs (3 sensors)
     tempTopology.push_back(3);

     bool ok;
     int x = QInputDialog::getInt(this, tr("Struktur des neuronalen Netzes"), tr("Wie viele hidden Layer wollen Sie?"), 3, 1, 10, 1, &ok);
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
     }
     //3 outputs (left straight right)
     tempTopology.push_back(3);
     topology = tempTopology;
     return topology;
    }
}

void SimulatorWindow::on_actionZu_Editor_wechseln_triggered()
{
    this->hide();
    pauseSimulation();
    this->parentWidget()->show();
}

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

void SimulatorWindow::connectSensorCalculation()
{
    //Automatische Abstandserkennung der Senoren
    connect(sensorsTimer, &QTimer::timeout, [this] {

    /* Die Idee dieser Abstandserkennungsfunktion besteht darin, dass Jeder Sensor iterativ betrachtet wird, dann die länge immer um eine feste anzahl iterativ erhöht wird
     * und bei jeder iteration geschaut wird, ob der sensor auf etwas (Obstacle oder MapPath) gestoßen ist. Falls dies der Fall ist, wird die Länge auf exakt diese Strecke zum nächsten Obstacle/Rand angepast.
     * Somit ist in den Sensoren immer die aktuelle länge gespeichert und sie lässt sich leicht (nahezu) kontinuierlich ausgeben*/
    for(int i=0; i<sv->getNumberOfSensors(); i++)
    {
        /* Hier wird die Sensorlänge hochgezählt. 1000 ist in dem Fall ein fester GrenzWert bei dem ich dachte, das höhere Werte dem System nicht mehr informationen geben würden (MapSize und GridSize beachten) - kann gerne noch kleiner angepasst werden*/
        for(int j=0;j<1000; j=j+10)
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

void SimulatorWindow::on_trainingModeButton_clicked()
{
    if(ui->trainingModeButton->isEnabled())
        ui->trainingModeButton->setEnabled(false);
    if(!ui->autonomousModeButton->isEnabled())
        ui->autonomousModeButton->setEnabled(true);
    trainingMode = true;

    //QMessageBox::about(this, "Trainingsmodus..", "Sie befinden sich im Trainingsmodus.Zum starten drücken sie oben 'Simulation starten'. Alle Bewegungsdaten werden aufgezeichnet. Das neuronale Netz wird während der Fahrt trainiert. Sie haben zusätzlich die möglichkeit das Netz mit den aufgenommenen Daten zu trainieren. Drücken Sie dazu anschließen oben auf den 'trainieren' Button und wählen Sie, wie oft trainiert werden soll. Das netz verbessert sich bei jeder Epoche.");

}

void SimulatorWindow::on_autonomousModeButton_clicked()
{
    if(!ui->trainingModeButton->isEnabled())
        ui->trainingModeButton->setEnabled(true);
    if(ui->autonomousModeButton->isEnabled())
        ui->autonomousModeButton->setEnabled(false);
    trainingMode = false;

    //QMessageBox::about(this, "Trainingsmodus..", "Sie befinden sich nun im autonomen Modus. Drücken Sie einfach Simulation starten und sehen Sie wie das neuronale Netz des Fahrzeugs gelernt hat.");

}

void SimulatorWindow::on_trainModelButton_clicked()
{
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

void SimulatorWindow::createNeuralNet(std::vector<unsigned> top)
{
    drivingNet = new neuralNet(topology);
}

void SimulatorWindow::on_actioncreateNeuralNet_triggered()
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
