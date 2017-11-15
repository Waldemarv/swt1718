#include "editor.h"
#include <iostream>

/*! Erstellt ein Editorfenster
 * \param *parent Fenster in welchem der Editor gezeichnet werden soll */
Editor::Editor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);

    m = nullptr;

    //Eine neue "Scene" wird erstellt. Hier werden unsere Items in einer festen Begrenzung geaddet.
    scene = new QGraphicsScene(this);
    //Die Scene wir der QGraphicsView hinzugefügt und angezeigt.
    ui->graphicsView->setScene(scene);

    //Hier wird das tree Widget auf 3 Spalten und die jeweiligen Breiten der Spalten angepasst
    ui->treeWidget->setColumnCount(3);
    ui->treeWidget->setColumnWidth(0,140);
    ui->treeWidget->setColumnWidth(1,60);
    ui->treeWidget->setColumnWidth(2,60);

}

Editor::~Editor()
{
    delete ui;
}
/*! Erstellt eine leere Map und oeffnet diese. */
void Editor::createMap()
{
    //Wenn keine map vorhanden
    if(m==nullptr) {
        //bool ob der ok button gedrückt wurde
        bool ok;

        //Setze die Größe auf die des QInputDialogs
        int x = QInputDialog::getInt(this, tr("Neue Map erstellen.."), tr("Mapgröße festlegen:"), 32, 16, 64, 1, &ok);
        if(ok) {

        //Erstelle eine Map mit den Größen
        m =  new Map(x,x);
        //Füge eine Map mit den Koordinaten auch dem TreeWidget hinzu
        addTreeMap(x,x);
        //Füge alle Items(Tiles, Obstacles) als "OberItem" hinzu.
        addTreeItems();
        //Setze die Größe der scene
        scene->setSceneRect((-(x*m->getGridSize())/2), -(x*m->getGridSize()/2),x*m->getGridSize()/2,x*m->getGridSize()/2);

        //Verbinde das tree Widget bei doppelclick mit einer LambdaFunktion, die ein QInputDialog öffnet und die Map Values mit der neuen Zahl updatet
        connect(ui->treeWidget, &QTreeWidget::doubleClicked, [this]{
            bool oki;
            int f = QInputDialog::getInt(this, tr("Größe Ändern"), tr("Neue Mapgröße festlegen:"), 32, 16, 64, 1, &oki);

            if(oki) {
                updateMapValues(f,f);
            }
        });
     }
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
            //erstelle eine neue Map
            createMap();
        }
        else if(reply == QMessageBox::No) {
            //lösche alle Elemente im TreeWidet
            clearTree();
            //Lösche die Map
            deleteMap();
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
    start.setAttribute("X:", QString::number(m->getStartingPoint().getX()));
    start.setAttribute("Y:", QString::number(m->getStartingPoint().getY()));
    points.appendChild(start);

    QDomElement end = document.createElement("EndingPoint");
    end.setAttribute("X:", QString::number(m->getEndingPoint().getX()));
    end.setAttribute("Y:", QString::number(m->getEndingPoint().getY()));
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
/*! Aktualisiert die Anzahl der Tiles im TreeView */
void Editor::updateTreeTiles()
{
    //Setze in der ersten Spalte neben dem Namen die Aktuelle anzahl von Tiles
    treeTiles->setText(1, QString::number(m->getNumberOfTiles()));
}
/*! Aktualisiert die Anzahl der Obstacles im TreeView */
void Editor::updateTreeObstacles()
{
    //Setze in der ersten Spalte neben dem Namen die Aktuelle anzahl von Obstacles
    treeObstacles->setText(1, QString::number(m->getNumberOfObstacles()));
}
/*! Aktualisiert die Groesse der Map im TreeView */
void Editor::updateTreeSize()
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
    updateTreeSize();
    //Setze die Größe der Scene im GraphicsView
    scene->setSceneRect((-(x*m->getGridSize())/2), -(x*m->getGridSize()/2),x*m->getGridSize()/2,x*m->getGridSize()/2);
    //Zeichne die umrandung der Scene neu.
    update();
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
        Tile *t = new straight((m->getNumberOfTiles()*25),0, 0);    //TODO: Ändern für Grid Layout

        //Füge es der Map hinzu
        m->addTile(t);
        //Füge es der scene hinzu und lass es damit zeichnen
        scene->addItem(t);
        //Lass die aktuelle Zahl der Tiles aktualisieren
        updateTreeTiles();
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
        Tile *t = new turn((m->getNumberOfTiles()*25),0, 0);  //TODO: Ändern für Grid Layout

        //Füge es der Map hinzu
        m->addTile(t);

        //Füge es der scene hinzu und lass es damit zeichnen
        scene->addItem(t);
        //Lass die aktuelle Zahl der Tiles aktualisieren
        updateTreeTiles();
        //Füge es dem Tree mit position hinzu
        addChild(treeTiles, "turn", m->getCurrentTile()->getPosition()->getX(), m->getCurrentTile()->getPosition()->getY());
    }

}

/*! Löscht das letzte erstellte Tile */
void Editor::on_deleteTile_clicked()
{
    if(m == nullptr) {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else if(m->getNumberOfTiles() == 0) {
        QMessageBox::about(this, "Keine Elemente vorhanden", "Es gibt nichts zu löschen!");
    }
    else {
        scene->removeItem(m->getCurrentTile());
        scene->update();
        m->deleteCurrentTile();

        updateTreeTiles();
        treeTiles->removeChild(treeTiles->child(treeTiles->childCount()-1));
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
        Obstacle *o = new Obstacle((m->getNumberOfObstacles()*20),0,10,10);
        m->addObstacle(o);
        scene->addItem(o);

        updateTreeObstacles();
        addChild(treeObstacles, "staticObstacle", m->getCurrentObstacle()->getPosition()->getX(), m->getCurrentObstacle()->getPosition()->getY());
    }
}

/*! Löscht das letzte erstelle Obstacle */
void Editor::on_deleteObstacle_clicked()
{
    if(m == nullptr) {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else if(m->getNumberOfObstacles() == 0) {
        QMessageBox::about(this, "Keine Elemente vorhanden", "Es gibt nichts zu löschen!");
    }
    else {
        scene->removeItem(m->getCurrentObstacle());
        scene->update();
        m->deleteCurrentObstacle();

        updateTreeObstacles();
        treeObstacles->removeChild(treeObstacles->child(treeObstacles->childCount()-1));

    }
}

/*! Löscht alle markierten Tiles */
void Editor::on_deleteSelectedTile_clicked()
{
    if(m == nullptr) {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else if(m->getNumberOfItems() == 0) {
        QMessageBox::about(this, "Keine Elemente vorhanden", "Es gibt nichts zu löschen!");
    }
    else {
        for(unsigned int i = 0; i < m->getNumberOfTiles() ; i++){
            if(m->getTile(i)->selected){
                scene->removeItem(m->getTile(i));
                scene->update();
                m->deleteTile(i);
                updateTreeTiles();
                treeTiles->removeChild(treeTiles->child(i));
            }
        }
    }
}

/*! Löscht alle markierten Obstacles */
void Editor::on_deleteSelectedObstacle_clicked()
{
    if(m == nullptr) {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else if(m->getNumberOfItems() == 0) {
        QMessageBox::about(this, "Keine Elemente vorhanden", "Es gibt nichts zu löschen!");
    }
    else {

        for(unsigned int i = 0; i < m->getNumberOfObstacles(); i++){
            if(m->getObstacle(i)->selected){
                scene->removeItem(m->getObstacle(i));
                scene->update();
                m->deleteObstacle(i);
                updateTreeObstacles();
                treeObstacles->removeChild(treeObstacles->child(i));
            }
        }
    }
}
