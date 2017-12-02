#ifndef CARETAKER_H
#define CARETAKER_H

#include "memento.h"

class Caretaker
{
private:
    std::vector<Memento*> mem;
public:
    Caretaker();
    Memento* getMemento();
    void setMemento(Memento* m);
    void deleteLast();

    int getMemSize();
};

#endif // CARETAKER_H
