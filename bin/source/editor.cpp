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
}

Editor::~Editor()
{
    delete ui;
    delete m;
}
/*! Erstellt eine leere Map und oeffnet diese. */
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
        //Verbindungsstück der Veränderung der scene
        connect(scene, QGraphicsScene::changed, [this]{
            for(unsigned int i=0; i<m->getNumberOfTiles();i++)
                setTreeTilesPosition(m->getTile(i)->scenePos(), i);

            for(unsigned int i=0; i<m->getNumberOfObstacles();i++)
                setTreeObstaclesPosition(m->getObstacle(i)->scenePos(), i);

            scene->update();
        });

        ui->graphicsView->setScene(scene);

        //Größe der Scene setzen
        scene->setSceneRect(0,0,x*m->getGridSize(),x*m->getGridSize());
        //GridLayout Linien zeichnen
        drawGridLayout(x,x);
        }

        //Verbinde das tree Widget bei doppelclick mit einer LambdaFunktion, die ein QInputDialog öffnet und die Map Values mit der neuen Zahl updatet
        connect(ui->treeWidget, &QTreeWidget::doubleClicked, [this]{
            bool oki;
            int f = QInputDialog::getInt(this, tr("Größe Ändern"), tr("Neue Mapgröße festlegen:"), 32, 16, 64, 1, &oki);

            if(oki) {
                updateMapValues(f,f);
            }
        });
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
/*! Speichert die geoeffnete Map in einer XML-Datei */
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
    //Pfad zum speichern wählen
    QString filename = QFileDialog::getSaveFileName(
            this,
            tr("Dateipfad wählen..."),
            "C:/MyMap.xml",
            tr("Extensible Markup Language Files (*.xml)")
);
    //File erstellen
    QFile file(filename);

    //Document erstellen
    QDomDocument document;

    //root (Map) festlegen
    QDomElement root = document.createElement("Map");

    //root dem document hinzufügen
    document.appendChild(root);

    //Unterkategorie Points einfügen
    QDomElement points = document.createElement("Points");
    root.appendChild(points);

    //Start- und Endingpoints als nodes dem document hinzufügen
    QDomElement start = document.createElement("StartingPoint");
    start.setAttribute("X:", QString::number(m->getStartingTile()->scenePos().x()));
    start.setAttribute("Y:", QString::number(m->getStartingTile()->scenePos().y()));
    points.appendChild(start);

    QDomElement end = document.createElement("EndingPoint");
    end.setAttribute("X:", QString::number(m->getEndingTile()->scenePos().x()));
    end.setAttribute("Y:", QString::number(m->getEndingTile()->scenePos().y()));
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
        node.setAttribute("lenght:", QString::number(m->getObstacle(i)->getlength()));
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
        tiles.appendChild(node);
    }

    //Die zu schreibene Datei erstellen/öffnen
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    //Dokument schreiben.
    QTextStream stream(&file);
    stream<<document.toString();
    }
}
/*! Laedt eine Map-Datei und oeffnet die Map im Editor */
void Editor::loadMap()
{
    //Pfad für die zu ladene Map wählen
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Datei wählen..."),
                "C:/",
                tr("Extensible Markup Language Files (*.xml)")
                );
}
/*! Loescht die geoffnete Map */
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
    treeTiles->setText(1, QString::number(m->getNumberOfTiles()));
}
/*! Aktualisiert die Anzahl der Obstacles im TreeView */
void Editor::updateTreeNumberOfObstacles()
{
    //Setze in der ersten Spalte neben dem Namen die Aktuelle anzahl von Obstacles
    treeObstacles->setText(1, QString::number(m->getNumberOfObstacles()));
}
/*! Aktualisiert die Groesse der Map im TreeView */
void Editor::updateTreeMapSize()
{
    //Setze die Größe der Map neu im Tree
    treeRoot->setText(1, QString::number(m->getSizeX()));
    treeRoot->setText(2, QString::number(m->getSizeY()));
}
/*! Aendert die Größe der Map und zeichnet diese neu
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
/*! Erstellt Ueberschriftelemente für das TreeView */
void Editor::addTreeItems()
{
    //Erstelle Tiles als Child von Map
    treeTiles = new QTreeWidgetItem();
    treeTiles->setText(0, "Tiles");
    treeTiles->setText(1,QString::number(treeTiles->childCount()));
    treeRoot->addChild(treeTiles);

    //Erstelle Obstacles als Child von Map
    treeObstacles = new QTreeWidgetItem();
    treeObstacles->setText(0, "Obstacles");
    treeObstacles->setText(1,QString::number(treeObstacles->childCount()));
    treeRoot->addChild(treeObstacles);

    //Weiterehinzufügen (SmartVehicle, Points, etc.)
}
/*! Setzt die geoeffnete Map als Hauptelement des TreeView */
void Editor::addTreeMap(double x, double y)
{
    //Setze Map als root des Trees
    treeRoot = new QTreeWidgetItem(ui->treeWidget);
    treeRoot->setText(0, "Map");
    treeRoot->setText(1,QString::number(x));
    treeRoot->setText(2,QString::number(y));
    ui->treeWidget->addTopLevelItem(treeRoot);
}
/*! Loescht alle Elemente des TreeView */
void Editor::clearTree()
{
    //Lösche root und alle Childs
    delete ui->treeWidget->topLevelItem(0);
}
/*! Fuegt dem TreeView ein Kindelement hinzu.
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

void Editor::keyPressEvent(QKeyEvent *event)
{
    //Starte die Timer wenn bestimmter Knopf gedrückt wird(Nur zu tetszwecken)
    if(event->key() == Qt::Key_Up)
        timer->start(10);

    else if(event->key() == Qt::Key_Left)
        leftTimer->start(10);

    else if(event->key() == Qt::Key_Right)
        rightTimer->start(10);

    QWidget::keyPressEvent(event);
}

void Editor::keyReleaseEvent(QKeyEvent *event)
{
    //Schalte Timer wieder aus, wenn Knopf losgelassen wird
    if(event->key() == Qt::Key_Up)
        timer->stop();
    else if(event->key() == Qt::Key_Left)
        leftTimer->stop();
    else if(event->key() == Qt::Key_Right)
        rightTimer->stop();

    QWidget::keyReleaseEvent(event);
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
      else {
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
        //Erstelle ein neues Tile mit Tile(x,y,ascent)
        Tile *t = new straight((m->getNumberOfTiles()*50),0, 0);
        //Füge es der Map hinzu
        m->addTile(t);
        //Füge es der scene hinzu und lass es damit zeichnen
        scene->addItem(t);
        //Lass die aktuelle Zahl der Tiles aktualisieren
        updateTreeNumberOfTiles();
        //Füge es dem Tree mit position hinzu
        addChild(treeTiles, "straight", m->getCurrentTile()->getPosition()->getX(), m->getCurrentTile()->getPosition()->getY());
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
        //Erstelle ein neues Tile mit Tile(x,y,ascent)
        Tile *t = new turn((m->getNumberOfTiles()*50),0, 0);
        //Füge es der Map hinzu
        m->addTile(t);
        //Füge es der scene hinzu und lass es damit zeichnen
        scene->addItem(t);
        //Lass die aktuelle Zahl der Tiles aktualisieren
        updateTreeNumberOfTiles();
        //Füge es dem Tree mit position hinzu
        addChild(treeTiles, "turn", m->getCurrentTile()->getPosition()->getX(), m->getCurrentTile()->getPosition()->getY());
    }

}

/*! Erstellt ein statisches Obstacle und fügt dieses der Map hinzu*/
void Editor::on_staticObstacle_clicked()
{
    if(m == nullptr)
    {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else
    {
        Obstacle *o = new Obstacle(m->getNumberOfObstacles()*20, 0, 20, 20);
        m->addObstacle(o);
        scene->addItem(o);


        updateTreeNumberOfObstacles();
        addChild(treeObstacles, "staticObstacle", m->getCurrentObstacle()->getPosition()->getX(), m->getCurrentObstacle()->getPosition()->getY());
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
        bool isOneTileSelected = false;
        for(unsigned int i = 0; i < m->getNumberOfTiles() ; i++){
            if(m->getTile(i)->selected){
                //Falls End oder Start Tiles gelöscht wurden, button wieder enabled um neue zu erstellen
                if(m->getTile(i)->getType() == "StartingTile")
                    ui->startingPointButton->setEnabled(true);
                if(m->getTile(i)->getType() == "EndingTile")
                   ui->endingPointButton->setEnabled(true);

                scene->removeItem(m->getTile(i));
                scene->update();
                m->deleteTile(i);
                updateTreeNumberOfTiles();
                treeTiles->removeChild(treeTiles->child(i));
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
                treeTiles->removeChild(treeTiles->child(treeTiles->childCount()-1));
            }
    }
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
        bool isOneObstacleSelected = false;
        for(unsigned int i = 0; i < m->getNumberOfObstacles(); i++){
            if(m->getObstacle(i)->selected){
                scene->removeItem(m->getObstacle(i));
                scene->update();
                m->deleteObstacle(i);
                updateTreeNumberOfObstacles();
                treeObstacles->removeChild(treeObstacles->child(i));
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
                treeObstacles->removeChild(treeObstacles->child(treeObstacles->childCount()-1));
            }
    }
}

void Editor::on_intersectionButton_clicked()
{
    if(m == nullptr)
    {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else
    {
        //Erstelle ein neues Tile mit Tile(x,y,ascent)
        Tile *t = new Intersection((m->getNumberOfTiles()*50),0, 0);
        //Füge es der Map hinzu
        m->addTile(t);
        //Füge es der scene hinzu und lass es damit zeichnen
        scene->addItem(t);
        //Lass die aktuelle Zahl der Tiles aktualisieren
        updateTreeNumberOfTiles();
        //Füge es dem Tree mit position hinzu
        addChild(treeTiles, "Intersection", m->getCurrentTile()->getPosition()->getX(), m->getCurrentTile()->getPosition()->getY());
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
        //Erstelle ein neues Tile mit Tile(x,y,ascent)
        Tile *t = new Tintersection((m->getNumberOfTiles()*50),0, 0);
        //Füge es der Map hinzu
        m->addTile(t);
        //Füge es der scene hinzu und lass es damit zeichnen
        scene->addItem(t);
        //Lass die aktuelle Zahl der Tiles aktualisieren
        updateTreeNumberOfTiles();
        //Füge es dem Tree mit position hinzu
        addChild(treeTiles, "T-Intersection", m->getCurrentTile()->getPosition()->getX(), m->getCurrentTile()->getPosition()->getY());
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
        Tile *s = Startingtile::createStartingTile();
        //setze aktuelles starting Tile
        m->setStartingTile(s);
        //Füge es der Map hinzu
        m->addTile(s);
        //Füge es der scene hinzu und lass es damit zeichnen
        scene->addItem(s);
        //Lass die aktuelle Zahl der Tiles aktualisieren
        updateTreeNumberOfTiles();
        //Füge es dem Tree mit position hinzu
        addChild(treeTiles, "StartingPoint", m->getCurrentTile()->getPosition()->getX(), m->getCurrentTile()->getPosition()->getY());

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
        Tile *e = Endingtile::createEndingTile();
        //Setze aktuelles Ending Tile
        m->setEndingTile(e);
        //Füge es der Map hinzu
        m->addTile(e);
        //Füge es der scene hinzu und lass es damit zeichnen
        scene->addItem(e);
        //Lass die aktuelle Zahl der Tiles aktualisieren
        updateTreeNumberOfTiles();
        //Füge es dem Tree mit position hinzu
        addChild(treeTiles, "EndingPoint", m->getCurrentTile()->getPosition()->getX(), m->getCurrentTile()->getPosition()->getY());

        ui->endingPointButton->setEnabled(false);
    }
}

void Editor::on_actionSimulation_starten_triggered()
{
    if(m->getStartingTile()!=0)
    {
    //SmartVehicle(int nangle, int nspeed, int nrotationSpeed, int x, int y)
    sv = new SmartVehicle(0,2,3,m->getStartingTile()->pos().x()+20, m->getStartingTile()->pos().y()+20);
    scene->addItem(sv);

    //Erstelle die 3 Timer für geradeaus/link/rechts um diese voneinander unabhängig zu steuern
    timer = new QTimer;
    leftTimer = new QTimer;
    rightTimer = new QTimer;

    //Verbinde die Timer mit der fortbewegung(advance) und auslenkung(left), (right)
    connect(timer, SIGNAL(timeout()), scene, SLOT(advance()));
    connect(leftTimer, QTimer::timeout, [this]{ sv->left(); });
    connect(rightTimer, QTimer::timeout, [this]{ sv->right(); });
    }
}
