#include "editor.h"
#include <iostream>

Editor::Editor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);

    m = nullptr;

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    ui->treeWidget->setColumnCount(3);
    ui->treeWidget->setColumnWidth(0,140);
    ui->treeWidget->setColumnWidth(1,60);
    ui->treeWidget->setColumnWidth(2,60);

}

Editor::~Editor()
{
    delete ui;
}

void Editor::createMap()
{
    //Wenn keine map vorhanden
    if(m==nullptr) {
        bool ok;
        int x = QInputDialog::getInt(this, tr("Neue Map erstellen.."), tr("Mapgröße festlegen:"), 32, 16, 64, 1, &ok);
        if(ok) {
        m =  new Map(x,x);

        root = new QTreeWidgetItem(ui->treeWidget);
        root->setText(0, "Map");
        root->setText(1,QString::number(x));
        root->setText(2,QString::number(x));
        ui->treeWidget->addTopLevelItem(root);

        }
  }
    //Wenn bereits map offen
    else {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Alte Map speichern?", "Möchten Sie vor dem Erstellen ihrer neuen Map die vorherige Karte speichern?",
                                      QMessageBox::Yes|QMessageBox::No|QMessageBox::Abort);
        if (reply == QMessageBox::Yes) {
            this->saveMap();
            m = nullptr;
            this->createMap();
        }
        else if(reply == QMessageBox::No) {
            m = nullptr;
            this->createMap();
        }
        else {
        }
    }
}

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
        node.setAttribute("X:", QString::number(m->getObstacle(i)->pos().x()));
        node.setAttribute("Y:", QString::number(m->getObstacle(i)->pos().y()));
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
        node.setAttribute("X:", QString::number(m->getTile(i)->pos().x()));
        node.setAttribute("Y:", QString::number(m->getTile(i)->pos().y()));
        tiles.appendChild(node);
    }

    //Die zu schreibene Datei erstellen/öffnen
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    //Dokument schreiben.
    QTextStream stream(&file);
    stream<<document.toString();
    }
}

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

void Editor::addChild(QTreeWidgetItem *parent, QString name, int posX, int posY)
{
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

void Editor::on_straightButton_clicked()
{
    if(m == nullptr)
    {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else
    {
        Tile *t = new straight((m->getNumberOfTiles()*50),0, 0);    //TODO: Ändern für Grid Layout
        m->addTile(t);
        scene->addItem(t);
        addChild(root, "straight", m->getCurrentTile()->getPosition()->getX(), m->getCurrentTile()->getPosition()->getY());
    }
}

void Editor::on_turnButton_clicked()
{
    if(m == nullptr)
    {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else
    {
        Tile *t = new turn((m->getNumberOfTiles()*50),0, 0);  //TODO: Ändern für Grid Layout
        m->addTile(t);
        scene->addItem(t);
        addChild(root, "turn", m->getCurrentTile()->getPosition()->getX(), m->getCurrentTile()->getPosition()->getY());
    }

}

void Editor::on_deleteButton_clicked()
{
    if(m == nullptr) {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else if(m->getNumberOfItems() == 0) {
        QMessageBox::about(this, "Keine Elemente vorhanden", "Es gibt nichts zu löschen!");
    }
    else {
        scene->removeItem(m->getCurrentTile());
        scene->update();
        m->deleteCurrentTile();
        root->removeChild(root->child(root->childCount()-1));
    }
}

void Editor::on_staticObstacle_clicked()
{
    if(m == nullptr)
    {
        QMessageBox::about(this, "Keine Map vorhanden", "Bitte erstellen Sie vor dem Bearbeiten eine Map");
    }
    else
    {
        Obstacle *o = new Obstacle(m->getNumberOfObstacles()*20,0,10,10);
        m->addObstacle(o);
        scene->addItem(o);
        addChild(root, "staticObstacle", m->getCurrentObstacle()->getPosition()->getX(), m->getCurrentObstacle()->getPosition()->getY());
    }
}
