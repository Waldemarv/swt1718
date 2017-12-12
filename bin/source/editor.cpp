#include "editor.h"
#include <iostream>
#include <QGraphicsWidget>

/*! Erstellt ein Editorfenster
 * \param *parent Fenster in welchem der Editor gezeichnet werden soll */
Editor::Editor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Editor)
    {
    ui->setupUi(this);

    m = nullptr;

    //TreeView erstellen
    ui->treeWidget->setColumnCount(3); // treeWidget mit 3 Zeilen erstellen
    ui->treeWidget->setHeaderLabels(QStringList() << "Objekt :" << "X : " << "Y : ");

    ui->treeWidget->setColumnWidth(0,140);
    ui->treeWidget->setColumnWidth(1,60);
    ui->treeWidget->setColumnWidth(2,60);

    caretaker = new Caretaker;
}

Editor::~Editor()
{
    delete ui;
    delete m;
}
/*! Erstellt eine leere Map und öffnet diese. */
void Editor::createMap()
{
    //Wenn keine Map vorhanden :
    if(m==nullptr) {
        bool ok; //Abfrage ob OK gedrückt wurde

        //Setze die Größe auf die des QInputDialogs
        int x = QInputDialog::getInt(this, tr("Neue Map erstellen.."), tr("Mapgröße festlegen:"), 16, 8, 64, 1, &ok);
        if(ok) { //Wenn OK gedrückt wurde :
        m =  new Map(x,x); //Neue Map erstellen
        addTreeMap(x,x); //Map dem TreeWidget hinzufügen
        addTreeItems(); //Hauptitems für das TreeWidget hinzufügen (Tiles, Obstacles...)

        //scene (graphisches Fenster zum letztendlichen zeichnen) erstellen und in das GraphicWidget setzen
        scene = new QGraphicsScene(this);
        //Verbindungsstelle der scene
        connectScene();
        //Verbindungsstelle des treeWidget
        connectTreeWidget();

        ui->graphicsView->setScene(scene);

        //Größe der Scene setzen
        scene->setSceneRect(0,0,x*m->getGridSize(),x*m->getGridSize());
        //GridLayout Linien zeichnen
        drawGridLayout(x,x);
        }

        //Wenn alte Map vorhanden war, müssen die Buttons für startingPoint/endingPoint wieder enabled werden
        if(!ui->startingPointButton->isEnabled())
            ui->startingPointButton->setEnabled(true);
        if(!ui->endingPointButton->isEnabled())
            ui->endingPointButton->setEnabled(true);
     }
    //Wenn bereits map offen
    else {
        //Dialog: "Möchten Sie vor dem Erstellen ihrer neuen Map die vorherige Karte speichern?"
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Alte Map speichern?", "Möchten Sie vor dem Erstellen ihrer neuen Map die vorherige Karte speichern?",
                                      QMessageBox::Yes|QMessageBox::No|QMessageBox::Abort);
        //Wenn ja gedrückt wurde:
        if (reply == QMessageBox::Yes) {
            //speicher die Map
            saveMap();
            //lösche alle Elemente im TreeWidet
            clearTree();
            //Lösche die Map
            deleteMap();
            //Lösch die Zeichnung
            delete scene;
            //erstelle eine neue Map
            createMap();
        }
        else if(reply == QMessageBox::No) {
            //lösche alle Elemente im TreeWidet
            clearTree();
            //Lösche die Map
            deleteMap();
            //Lösch die Zeichnung
            delete scene;
            //erstelle eine neue Map
            createMap();
        }
        else {
        }
    }
}
/*! Speichert die geöffnete Map in einer XML-Datei */
void Editor::saveMap()
{
    //Map vorhanden?
    if(m==nullptr){
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Speichern eine Map!");
    }
    else if(m->getNumberOfItems() == 0){
        QMessageBox::about(this, "Keine Elemente in der Map", "Bitte fügen Sie mindestens ein Objekt hinzu!");
    }
    else{
        if(m->getStartingTile() == NULL)
        {
            QMessageBox::about(this, "Kein Startpunkt", "Bitte erstellen Sie vor dem Speichern einen Startpunkt!");
        }
        else if(m->getEndingTile() == NULL)
        {
            QMessageBox::about(this, "Kein Endpunkt", "Bitte erstellen Sie vor dem Speichern einen Endpunkt!");
        }
        else
        {
    //Pfad zum speichern wählen
    QString currentPath = QDir::currentPath();
    currentPath.append("/source/Maps/Map.xml");

    QString filename = QFileDialog::getSaveFileName(
            this,
            tr("Dateipfad wählen..."),
            currentPath,
            tr("Extensible Markup Language Files (*.xml)")
);
    if(filename != NULL)
    {
    //File erstellen
    QFile file(filename);

    //Document erstellen
    QDomDocument document;

    //root (Map) festlegen
    QDomElement root = document.createElement("Map");

    //root dem document hinzufügen
    document.appendChild(root);

    //Daten der Map Einfügen
    QDomElement data = document.createElement("Data");
    root.appendChild(data);

    //Mapgröße als Node Einfügen
    QDomElement size = document.createElement("MapSize");
    size.setAttribute("X:", QString::number(m->getSizeX()));
    size.setAttribute("Y:",QString::number(m->getSizeY()));
    data.appendChild(size);

    //Unterkategorie Points einfügen
    QDomElement points = document.createElement("Points");
    root.appendChild(points);

    //Start- und Endingpoints als nodes dem document hinzufügen
    QDomElement start = document.createElement("StartingPoint");
    start.setAttribute("X:", QString::number(m->getStartingTile()->scenePos().x()+20));
    start.setAttribute("Y:", QString::number(m->getStartingTile()->scenePos().y()+20));
    points.appendChild(start);

    QDomElement end = document.createElement("EndingPoint");
    end.setAttribute("X:", QString::number(m->getEndingTile()->scenePos().x()+20));
    end.setAttribute("Y:", QString::number(m->getEndingTile()->scenePos().y()+20));
    points.appendChild(end);

    //QDomElement smartVehicle = document.createElement("SmartVehicle");
    //smartVehicle.setAttribute("Speed:", QString::number(m->getSmartVehicle.getSpeed()));

    //Unterkategorie für obstacles
    QDomElement obstacles = document.createElement("Obstacles");
    root.appendChild(obstacles);

    //Obstacles als nodes dem unterpunkt hinzufügen
    for(unsigned int i = 0; i < m->getNumberOfObstacles(); i++)
    {
        QDomElement node = document.createElement("Obstacle");
        node.setAttribute("ID:", QString::number(i));
        node.setAttribute("Typ:", m->getObstacle(i)->getType());
        node.setAttribute("width:", QString::number(m->getObstacle(i)->getwidth()));
        node.setAttribute("length:", QString::number(m->getObstacle(i)->getlength()));
        node.setAttribute("X:", QString::number(m->getObstacle(i)->scenePos().x()));
        node.setAttribute("Y:", QString::number(m->getObstacle(i)->scenePos().y()));
        obstacles.appendChild(node);
    }

    //Unterpunkt für tiles hinzufügen
    QDomElement tiles = document.createElement("Tiles");
    root.appendChild(tiles);

    //Tiles als nodes dem document hinzufügen
    for(unsigned int i=0; i < m->getNumberOfTiles(); i++)
    {
        QDomElement node = document.createElement("Tile");
        node.setAttribute("Typ:", m->getTile(i)->getType());
        node.setAttribute("ID:", QString::number(i));
        node.setAttribute("ascent:", QString::number(m->getTile(i)->getAscent()));
        node.setAttribute("X:", QString::number(m->getTile(i)->scenePos().x()));
        node.setAttribute("Y:", QString::number(m->getTile(i)->scenePos().y()));
        node.setAttribute("direction:", QString::number(m->getTile(i)->getDirection()));
        tiles.appendChild(node);
    }

    //Die zu schreibene Datei erstellen/öffnen
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    //Dokument schreiben.
    QTextStream stream(&file);
    stream<<document.toString();
    }
    }
    }
}
/*! Lädt eine Map-Datei und öffnet die Map im Editor */
void Editor::loadMap()
{
    if(m==nullptr) {

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    //Dokument für Datei erstellen
    QDomDocument document;
    //Pfad für die zu ladene Map wählen
    QString currentPath = QDir::currentPath();
    currentPath.append("/source/Maps");

    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Datei wählen..."),
                currentPath,
                tr("Extensible Markup Language Files (*.xml)")
                );

    if(filename != NULL)
    {
        //Ausgewählte Datei öffnen
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this,tr("Datei kann nicht geöffnet werden."),file.errorString());
            return;
        }
        //File in QDomDocument laden und file wieder schließen
        document.setContent(&file);
        file.close();

        //Verbindungsstelle für scene
        connectScene();

        //Verbindungsstelle für treeWidget
        connectTreeWidget();

        //Root erstellen
        QDomElement root = document.firstChildElement();
        //Liste aller sich im root befindenden Nodes erstellen
        QDomNodeList allNodes = root.childNodes();
        //Data als erstes darunterliegendes Node der liste erstellen
        QDomNode data = allNodes.at(0);
        //Mapsize als Element erstellen
        QDomElement mapSize = data.firstChild().toElement();
        //Attribute auslesen
        int sizeX = mapSize.attribute("X:").toInt();
        int sizeY = mapSize.attribute("Y:").toInt();
        //map erstellen
        m = new Map(sizeX, sizeY);
        //TreeView hinzufügen
        addTreeMap(sizeX,sizeY);
        addTreeItems();
        //Scenegröße definieren
        scene->setSceneRect(0,0,sizeX*m->getGridSize(), sizeY*m->getGridSize());
        //GridLayoutZeichnen
        drawGridLayout(sizeX,sizeY);

        //Points als zweite darunterliegende Liste erstellen
        QDomNodeList points = allNodes.at(1).childNodes();
        //start und endpunkte als darunterliegende elemente erstellen
        QDomElement startingPoint = points.at(0).toElement();
        QDomElement endingPoint = points.at(1).toElement();

        //X und Y herausfinden
        int startingPointX = startingPoint.attribute("X:").toInt();
        int startingPointY = startingPoint.attribute("Y:").toInt();
        //setzen
        m->setStartingPoint(startingPointX,startingPointY);

        int endingPointX = endingPoint.attribute("X:").toInt();
        int endingPointY = endingPoint.attribute("Y:").toInt();

        m->setEndingPoint(endingPointX, endingPointY);

        QDomNodeList obstacles = allNodes.at(2).childNodes();

        for(int i = 0; i<obstacles.count(); i++)
        {
            QDomElement obstaclesAt = obstacles.at(i).toElement();

            int obstacleX = obstaclesAt.attribute("X:").toInt();
            int obstacleY = obstaclesAt.attribute("Y:").toInt();
            int obstacleWidth = obstaclesAt.attribute("width:").toInt();
            int obstacleLength = obstaclesAt.attribute("length:").toInt();

            if(obstaclesAt.attribute("Typ:") == "staticObstacle")
            {
            Obstacle *o = new Obstacle(obstacleX, obstacleY, obstacleWidth,obstacleLength);
            scene->addItem(o);
            m->addObstacle(o);
            }
            //elseif(obstaclesAt.attribute("Typ:" == "dynamicObstacle") TODO: Für dynamic Obstacle ergänzen

            updateTreeNumberOfObstacles();
        }

        QDomNodeList tiles = allNodes.at(3).childNodes();

        for(int i = 0; i<tiles.count(); i++)
        {
            QDomElement tilesAt = tiles.at(i).toElement();

            int tileX = tilesAt.attribute("X:").toInt();
            int tileY = tilesAt.attribute("Y:").toInt();
            int tileAscent = tilesAt.attribute("ascent:").toInt();
            int tileDirection = tilesAt.attribute("direction:").toInt();

            if(tilesAt.attribute("Typ:") == "StartingTile"){
                Tile* t = Startingtile::createStartingTile(tileX, tileY, tileAscent, tileDirection);
                ui->startingPointButton->setEnabled(false);
                m->setStartingTile(t);
                m->addTile(t);
                scene->addItem(t);
            }

            else if(tilesAt.attribute("Typ:") == "EndingTile"){
                Tile* t = Endingtile::createEndingTile(tileX, tileY, tileAscent, tileDirection);
                ui->endingPointButton->setEnabled(false);
                m->addTile(t);
                m->setEndingTile(t);
                scene->addItem(t);
            }

            else if(tilesAt.attribute("Typ:") == "straight"){
                Tile* t = new straight(tileX, tileY, tileAscent, tileDirection);
                scene->addItem(t);
                m->addTile(t);
            }
            else if(tilesAt.attribute("Typ:") == "turn"){
                Tile* t = new turn(tileX, tileY, tileAscent, tileDirection);
                scene->addItem(t);
                m->addTile(t);
            }

            else if(tilesAt.attribute("Typ:") == "T-Intersection"){
                Tile* t = new Tintersection(tileX, tileY, tileAscent, tileDirection);
                scene->addItem(t);
                m->addTile(t);
            }

            else if(tilesAt.attribute("Typ:") == "Intersection"){
                Tile* t = new Intersection(tileX, tileY, tileAscent);
                scene->addItem(t);
                m->addTile(t);
            }
            updateTreeNumberOfTiles();
            }
        }
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Vor dem schliessen speichern?", "Möchten Sie das Projekt vor dem schließen speichern?",
                                      QMessageBox::Yes|QMessageBox::No|QMessageBox::Abort);
        if (reply == QMessageBox::Yes) {
            //speicher die Map
            saveMap();
            //lösche alle Elemente im TreeWidet
            clearTree();
            //Lösche die Map
            deleteMap();
            //Lösch die Zeichnung
            delete scene;
            //erstelle eine neue Map
            loadMap();
        }
        else if(reply == QMessageBox::No) {
            //lösche alle Elemente im TreeWidet
            clearTree();
            //Lösche die Map
            deleteMap();
            //Lösch die Zeichnung
            delete scene;
            //erstelle eine neue Map
            loadMap();
        }
    }
}
/*! Löscht die geoffnete Map */
void Editor::deleteMap()
{
    //Lösche m
    delete m;
    //Setze m zu nullptr für if Abfragen in saveMap und createMap.
    m=nullptr;
}

void Editor::drawGridLayout(int x, int y)
{
    //Pen für das Zeichnen des Rasters erstellen
    QPen pen;
    pen.setColor(Qt::lightGray);
    pen.setWidth(0.5);

    //Koordinatenraster zeichnen
    for (int i = 0 ; i<=x ; i++)
        scene->addLine(0,i*m->getGridSize(),x*m->getGridSize(),i*m->getGridSize(),pen);
    for(int i = 0; i<=y; i++)
        scene->addLine(i*m->getGridSize(),0,i*m->getGridSize(),y*m->getGridSize(),pen);
}
/*! Aktualisiert die Anzahl der Tiles im TreeView */
void Editor::updateTreeNumberOfTiles()
{
    //Setze in der ersten Spalte neben dem Namen die Aktuelle anzahl von Tiles
    treeTiles->setText(0, "Tiles [" + QString::number(m->getNumberOfTiles()) + "]");
}
/*! Aktualisiert die Anzahl der Obstacles im TreeView */
void Editor::updateTreeNumberOfObstacles()
{
    //Setze in der ersten Spalte neben dem Namen die Aktuelle anzahl von Obstacles
    treeObstacles->setText(0, "Obstacles [" + QString::number(m->getNumberOfObstacles()) + "]");
}
/*! Aktualisiert die Größe der Map im TreeView */
void Editor::updateTreeMapSize()
{
    //Setze die Größe der Map neu im Tree
    treeRoot->setText(1, QString::number(m->getSizeX()));
    treeRoot->setText(2, QString::number(m->getSizeY()));
}
/*! Ändert die Größe der Map und zeichnet diese neu
    \param x neuer x-Wert der Map
    \param y neuer y-Wert der Map*/
void Editor::updateMapValues(int x, int y)
{
    //Setze die Größe der Map
    m->setSize(x,y);
    //Update die Größe im Tree
    updateTreeMapSize();
    //Setze die Größe der Scene im GraphicsView
    scene->setSceneRect(0,0,x*m->getGridSize(),y*m->getGridSize());
    //Zeichne die Linien für das neue GridLayout
    drawGridLayout(x,y);

}
/*! Erstellt Überschriftelemente für das TreeView */
void Editor::addTreeItems()
{
    //Erstelle Tiles als Child von Map
    treeTiles = new QTreeWidgetItem();
    treeTiles->setText(0, "Tiles");
    //treeTiles->setText(1,QString::number(treeTiles->childCount()));
    treeRoot->addChild(treeTiles);

    //Erstelle Obstacles als Child von Map
    treeObstacles = new QTreeWidgetItem();
    treeObstacles->setText(0, "Obstacles");
    //treeObstacles->setText(1,QString::number(treeObstacles->childCount()));
    treeRoot->addChild(treeObstacles);
}
/*! Setzt die geöffnete Map als Hauptelement des TreeView */
void Editor::addTreeMap(double x, double y)
{
    //Setze Map als root des Trees
    treeRoot = new QTreeWidgetItem(ui->treeWidget);
    treeRoot->setText(0, "Map");
    treeRoot->setText(1,QString::number(x));
    treeRoot->setText(2,QString::number(y));
    ui->treeWidget->addTopLevelItem(treeRoot);
}
/*! Löscht alle Elemente des TreeView */
void Editor::clearTree()
{
    //Lösche root und alle Childs
    delete ui->treeWidget->topLevelItem(0);
}
/*! Fügt dem TreeView ein Kindelement hinzu.
 *  \param *param zeigt auf das TreeView
 *  \param name Typ des Elementes (Tile,Obstacle..)
 *  \param posX x-Position des Elementes
 *  \param posY y-Position des Elementes */
void Editor::addChild(QTreeWidgetItem *parent, QString name, int posX, int posY)
{
    //Füge einem Parent Item ein Child hinzu
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, name);
    item->setText(1, QString::number(posX));
    item->setText(2, QString::number(posY));
    parent->addChild(item);
}

void Editor::connectScene()
{
    connect(scene, &QGraphicsScene::changed, [this]{

        for(unsigned int i=0; i<m->getNumberOfTiles();i++)
        {
            if(m->getTile(i)->isClicked()){
                caretaker->setMemento(m->createMemento());
                m->getTile(i)->setClicked(false);
            }
            m->getTile(i)->fitIntoGrid();
            m->getTile(i)->fitIntoScene();

//            for(unsigned int j=0; j<m->getNumberOfTiles();j++){
//                if(i=!j && !(m->getTile(i)->boundingRect().intersects(m->getTile(j)->boundingRect()))){
//                    QMessageBox::warning(this,"Warning!!"," Tiles are intersecting !!");
//                }
//            }

        }
        for(unsigned int i=0; i<m->getNumberOfObstacles();i++)
        {
            if(m->getObstacle(i)->isClicked())
            {
                caretaker->setMemento(m->createMemento());
                m->getObstacle(i)->setClicked(false);
            }
        }

        scene->update();
    });
}

void Editor::connectTreeWidget()
{
    connect(ui->treeWidget, &QTreeWidget::doubleClicked, [this]{
        bool oki;
        int f = QInputDialog::getInt(this, tr("Größe Ändern"), tr("Neue Mapgröße festlegen:"), 32, 16, 64, 1, &oki);

        if(oki) {
            updateMapValues(f,f);
        }
    });
}

/*void Editor::autoSave()
{
    autoSaveTimer = new QTimer;
    connect(autoSaveTimer, &QTimer::timeout, this, [this] {
        if(caretaker->getMemento() != m->createMemento())
        {
            setMemento(m->createMemento());
            qDebug()<<"impulse";
        }
    });
}*/

void Editor::keyPressEvent(QKeyEvent *event)
{
    caretaker->setMemento(m->createMemento());
}


void Editor::setTreeTilesPosition(QPointF position, int index)
{
    treeTiles->child(index)->setText(1, QString::number(position.x()));
    treeTiles->child(index)->setText(2, QString::number(position.y()));
}

void Editor::setTreeObstaclesPosition(QPointF position, int index)
{
    treeObstacles->child(index)->setText(1, QString::number(position.x()));
    treeObstacles->child(index)->setText(2, QString::number(position.y()));
}

void Editor::on_actionMap_Laden_triggered()
{
    this->loadMap();
}

void Editor::on_actionMap_Speichern_triggered()
{
    this->saveMap();
}

void Editor::on_actionNeue_Map_triggered()
{
    this->createMap();
}

/*! Schließt das Editorfenster und fragt ggf. ab , was mit der geöffneten Map passieren soll */
void Editor::on_actionSchliessen_triggered()
{
      //Vor dem schließen Ja/Nein/Abbrechen Abfrage
      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Vor dem schliessen speichern?", "Möchten Sie das Projekt vor dem schließen speichern?",
                                    QMessageBox::Yes|QMessageBox::No|QMessageBox::Abort);
      if (reply == QMessageBox::Yes) {
          this->saveMap();
          QApplication::quit();
      }
      else if(reply == QMessageBox::No) {
          QApplication::quit();
      }
}

/*! Erstellt ein neues Straigt-Tile und fügt dieses der Map hinzu */
void Editor::on_straightButton_clicked()
{
    //Wenn keine Map vorhanden
    if(m == nullptr)
    {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else
    {
        //Für Undo
        caretaker->setMemento(m->createMemento());

        //Erstelle ein neues Tile mit Tile(x,y,ascent)
        Tile *t = new straight((m->getNumberOfTiles()*m->getGridSize()),0, 0,0);
        //Füge es der Map hinzu
        m->addTile(t);
        //Füge es der scene hinzu und lass es damit zeichnen
        scene->addItem(t);
        //Lass die aktuelle Zahl der Tiles aktualisieren
        updateTreeNumberOfTiles();
    }
}

/*! Erstellt ein neues Turn-Tile und fügt dieses der Map hinzu */
void Editor::on_turnButton_clicked()
{
    if(m == nullptr)
    {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else
    {
        //Für Undo
        caretaker->setMemento(m->createMemento());
        //Erstelle ein neues Tile mit Tile(x,y,ascent)
        Tile *t = new turn((m->getNumberOfTiles()*m->getGridSize()),0, 0,0);
        //Füge es der Map hinzu
        m->addTile(t);
        //Füge es der scene hinzu und lass es damit zeichnen
        scene->addItem(t);
        //Lass die aktuelle Zahl der Tiles aktualisieren
        updateTreeNumberOfTiles();
    }

}

/*! Erstellt ein statisches Obstacle und fügt dieses der Map hinzu*/
void Editor::on_staticObstacleButton_clicked()
{
    if(m == nullptr)
    {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else
    {
        //Für Undo
        caretaker->setMemento(m->createMemento());
        Obstacle *o = new Obstacle(m->getNumberOfObstacles()*(m->getGridSize()/2), 0, 8, 8);
        m->addObstacle(o);
        scene->addItem(o);


        updateTreeNumberOfObstacles();
    }
}

/*! Löscht alle markierten Tiles, falls keins markiert ist, das letzte (Zummaenfügung von beiden delete Funktionen) */
void Editor::on_deleteTileButton_clicked()
{
    if(m == nullptr) {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else if(m->getNumberOfItems() == 0) {
        QMessageBox::about(this, "Keine Elemente vorhanden", "Es gibt nichts zu löschen!");
    }
    else {
        //Für Undo
        caretaker->setMemento(m->createMemento());

        bool isOneTileSelected = false;
        for(unsigned int i = 0; i < m->getNumberOfTiles() ; i++){
            if(m->getTile(i)->isSelected()){
                //Falls End oder Start Tiles gelöscht wurden, button wieder enabled um neue zu erstellen
                if(m->getTile(i)->getType() == "StartingTile")
                    ui->startingPointButton->setEnabled(true);
                if(m->getTile(i)->getType() == "EndingTile")
                   ui->endingPointButton->setEnabled(true);

                scene->removeItem(m->getTile(i));
                scene->update();
                m->deleteTile(i);
                updateTreeNumberOfTiles();
                i--;
                isOneTileSelected = true;
            }
        }
            if(isOneTileSelected == false)
            {
                //Falls End oder Start Tiles gelöscht wurden, button wieder enabled um neue zu erstellen
                if(m->getCurrentTile()->getType() == "StartingTile")
                    ui->startingPointButton->setEnabled(true);
                if(m->getCurrentTile()->getType() == "EndingTile")
                   ui->endingPointButton->setEnabled(true);

                scene->removeItem(m->getCurrentTile());
                scene->update();
                m->deleteCurrentTile();

                updateTreeNumberOfTiles();
            }
    }
    scene->update();
}

/*! Löscht alle markierten Obstacles, falls keins markiert ist, das letzte (Zummaenfügung von beiden delete Funktionen) */
void Editor::on_deleteObstacleButton_clicked()
{
    if(m == nullptr) {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else if(m->getNumberOfItems() == 0) {
        QMessageBox::about(this, "Keine Elemente vorhanden", "Es gibt nichts zu löschen!");
    }
    else {
        //Für Undo
        caretaker->setMemento(m->createMemento());

        bool isOneObstacleSelected = false;
        for(unsigned int i = 0; i < m->getNumberOfObstacles(); i++){
            if(m->getObstacle(i)->isSelected()){
                scene->removeItem(m->getObstacle(i));
                scene->update();
                m->deleteObstacle(i);
                updateTreeNumberOfObstacles();
                i--;
                isOneObstacleSelected = true;
            }
        }
            if(isOneObstacleSelected == false)
            {
                scene->removeItem(m->getCurrentObstacle());
                scene->update();
                m->deleteCurrentObstacle();

                updateTreeNumberOfObstacles();
            }
    }
    scene->update();
}

void Editor::on_intersectionButton_clicked()
{
    if(m == nullptr)
    {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else
    {
        //Für Undo
        caretaker->setMemento(m->createMemento());
        //Erstelle ein neues Tile mit Tile(x,y,ascent)
        Tile *t = new Intersection((m->getNumberOfTiles()*m->getGridSize()),0, 0);
        //Füge es der Map hinzu
        m->addTile(t);
        //Füge es der scene hinzu und lass es damit zeichnen
        scene->addItem(t);
        //Lass die aktuelle Zahl der Tiles aktualisieren
        updateTreeNumberOfTiles();
        //Füge es dem Tree mit position hinzu
    }
}

void Editor::on_tIntersectionButton_clicked()
{
    if(m == nullptr)
    {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else
    {
        //Für Undo
        caretaker->setMemento(m->createMemento());
        //Erstelle ein neues Tile mit Tile(x,y,ascent)
        Tile *t = new Tintersection((m->getNumberOfTiles()*m->getGridSize()),0, 0,0);
        //Füge es der Map hinzu
        m->addTile(t);
        //Füge es der scene hinzu und lass es damit zeichnen
        scene->addItem(t);
        //Lass die aktuelle Zahl der Tiles aktualisieren
        updateTreeNumberOfTiles();
        //Füge es dem Tree mit position hinzu
    }
}

void Editor::on_startingPointButton_clicked()
{
    if(m == nullptr)
    {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else
    {
        //Für Undo
        caretaker->setMemento(m->createMemento());
        Tile *s = Startingtile::createStartingTile(0,0,0,0);
        //setze aktuelles starting Tile
        m->setStartingTile(s);
        //Füge es der Map hinzu
        m->addTile(s);
        //Füge es der scene hinzu und lass es damit zeichnen
        scene->addItem(s);
        //Lass die aktuelle Zahl der Tiles aktualisieren
        updateTreeNumberOfTiles();
        //Füge es dem Tree mit position hinzu

        ui->startingPointButton->setEnabled(false);
    }
}

void Editor::on_endingPointButton_clicked()
{
    if(m == nullptr)
    {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else
    {
        //Für Undo
        caretaker->setMemento(m->createMemento());
        Tile *e = Endingtile::createEndingTile(0,300,0,0);
        //Setze aktuelles Ending Tile
        m->setEndingTile(e);
        //Füge es der Map hinzu
        m->addTile(e);
        //Füge es der scene hinzu und lass es damit zeichnen
        scene->addItem(e);
        //Lass die aktuelle Zahl der Tiles aktualisieren
        updateTreeNumberOfTiles();
        //Füge es dem Tree mit position hinzu

        ui->endingPointButton->setEnabled(false);
    }// TODO
}

void Editor::on_actionSimulation_starten_triggered()
{
    if(m!=0)
    {
        if(m->getStartingTile()== 0 || m->getEndingTile() == 0) {
            QMessageBox::about(this, "Kein Start- oder Endpunkt vorhanden", "Bitte erstellen Sie vor der Simulation einen Start- und Endpunkt");        }
        else {
            //TODO: Wenn simulation offen, fragen ob man fortsetzen möchte oder eine neue Simulation starten will.

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Darstellung der Simulation", "Möchten Sie eine graphische Visualisierung für die Simulation?",
                                          QMessageBox::Yes|QMessageBox::No|QMessageBox::Abort);

            if (reply == QMessageBox::No) {
                m->setMapPath();
                m->setStartingPoint(m->getStartingTile()->x()+20, m->getStartingTile()->y()+20);
                SimulatorCMDL *cmdl = new SimulatorCMDL(*m);
                this->hide();
            }
            if (reply == QMessageBox::Yes) {
                m->setMapPath();
                m->setStartingPoint(m->getStartingTile()->x()+20, m->getStartingTile()->y()+20);
                SimulatorWindow *sim = new SimulatorWindow(*m,this);
                sim->show();
                this->hide();
            }
        }
    }
    else
    {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor der Simulation eine Map!");
    }
}

void Editor::on_actionUndo_triggered()
{
    //Wenn es ein Memento gibt
    if(caretaker->getMemSize() > 0)
    {
        //Entferne alle alten Tiles aus der scene
        for(unsigned int i = 0; i<m->getNumberOfTiles();i++)
            scene->removeItem(m->getTile(i));

        for(unsigned int i = 0; i<m->getNumberOfObstacles();i++)
            scene->removeItem(m->getObstacle(i));

        //setze das neue memento als aktuelle Map
        m->setMemento(caretaker->getMemento());
        //lösche das letzte memento (speicherreservierung)
        caretaker->deleteLast();

        bool b_endingTile = false;
        bool b_startingTile = false;

        for(unsigned int i = 0; i<m->getNumberOfTiles();i++)
        {
            //Falls start- oder endpunkte betroffen speichere ob sie behalten werden
            if(m->getTile(i)->getType() == "StartingTile")
               b_startingTile = true;
            if(m->getTile(i)->getType() == "EndingTile")
               b_endingTile = true;

            //Füge die Elemente der scene hinzu
            scene->addItem(m->getTile(i));
        }

        for(unsigned int i = 0; i<m->getNumberOfObstacles(); i++)
            scene->addItem(m->getObstacle(i));

        //Setze die entsprechenden buttons wieder auf enabled
        ui->startingPointButton->setEnabled(!b_startingTile);
        ui->endingPointButton->setEnabled(!b_endingTile);

        updateTreeNumberOfTiles();
        updateTreeNumberOfObstacles();

        scene->update();
    }
    else
    {
        QMessageBox::about(this, "Kein Memento", "Vor dem Rückgängig machen bitte erst eine Änderung vornehmen !");
    }
}

void Editor::on_dynamicObstacleButton_clicked()
{
    if(m == nullptr)
    {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else
    {
        //Für Undo
        caretaker->setMemento(m->createMemento());

        Obstacle *o = new DynamicObstacle(m->getNumberOfObstacles()*(m->getGridSize()/2), 0, 5, 5, 0);
        m->addObstacle(o);
        scene->addItem(o);

        updateTreeNumberOfObstacles();
    }

}
