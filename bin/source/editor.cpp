 #include "editor.h"
#include <iostream>

Editor::Editor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);
    m = new Map();
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
        m =  new Map(x,x);
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
        else if(reply == QMessageBox::Abort) {

        }
        else {
            m = nullptr;
            this->createMap();
        }
    }
}

void Editor::saveMap()
{
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

    //Start- und Endingpoints als nodes dem document hinzufügen
    QDomElement start = document.createElement("StartingPoint");
    start.setAttribute("X:", QString::number(m->getStartingPoint().getX()));
    start.setAttribute("Y:", QString::number(m->getStartingPoint().getY()));
    root.appendChild(start);

    QDomElement end = document.createElement("EndingPoint");
    end.setAttribute("X:", QString::number(m->getEndingPoint().getX()));
    end.setAttribute("Y:", QString::number(m->getEndingPoint().getY()));
    root.appendChild(end);

    //Obstacles als nodes dem document hinzufügen
    for(int i=0; i < m->getNumberOfObstacles(); i++)
    {
        QDomElement node = document.createElement("Obstacle");
        node.setAttribute("ID:", QString::number(i));
        node.setAttribute("width:", QString::number(m->getObstacle(i).getwidth()));
        node.setAttribute("lenght:", QString::number(m->getObstacle(i).getlength()));
        node.setAttribute("X:", QString::number(m->getObstacle(i).getPosition().getX()));
        node.setAttribute("Y:", QString::number(m->getObstacle(i).getPosition().getY()));
    }

    //Tiles als nodes dem document hinzufügen
    for(int i=0; i < m->getNumberOfTiles(); i++)
    {
        QDomElement node = document.createElement("Tile");
        node.setAttribute("ID:", QString::number(i));
        node.setAttribute("ascent:", QString::number(m->getTile(i).getAscent()));
        node.setAttribute("X:", QString::number(m->getTile(i).getPosition().getX()));
        node.setAttribute("Y:", QString::number(m->getTile(i).getPosition().getY()));

    }

    //Die zu schreibene Datei erstellen/öffnen
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    //Dokument schreiben.
    QTextStream stream(&file);
    stream<<document.toString();
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
      else if(reply == QMessageBox::Abort) {

      }
      else {
          QApplication::quit();
      }
}
