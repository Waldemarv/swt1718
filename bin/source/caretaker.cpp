#include "caretaker.h"

Caretaker::Caretaker()
{

}

Memento *Caretaker::getMemento()
{
    return mem.back();
}

void Caretaker::setMemento(Memento *m)
{
    mem.push_back(m);
}

void Caretaker::deleteLast()
{
    mem.pop_back();
}

int Caretaker::getMemSize()
{
    return mem.size();
}
