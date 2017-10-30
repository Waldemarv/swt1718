#include "editor.h"
#include "ui_editor.h"
#include "map.h"
Editor::Editor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);
}

Editor::~Editor()
{
    delete ui;
}

void Editor::on_actionNeue_Map_triggered()
{
    Map* m = new Map();
}

void Editor::on_actionSchliessen_triggered()
{
    //TODO : Map speichern/verwerfen Fenster!
    close();
}
