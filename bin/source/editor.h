#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>

namespace Ui {
class Editor;
}

class Editor : public QMainWindow
{
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = 0);
    ~Editor();

private slots:
    void on_actionNeue_Map_triggered();

    void on_actionSchliessen_triggered();

private:
    Ui::Editor *ui;
};

#endif // EDITOR_H
