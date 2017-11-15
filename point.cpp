#include "point.h"
/*! Erstellt einen neuen Punkt im Koordinatenursprung */
Point::Point() {
    this->x = 0;
    this->y = 0;
}

Point::~Point() {
}

/*! Erstellt einen Punkt
 * \param nx x-Koordinate des Punktes
 * \param ny y-Koordinate des Punktes
 * \returns Punkt mit den Koordinaten (nx,ny) */
Point::Point(double nx, double ny) {
    x=nx;
    y=ny;
}
/*! Verändert die x-Koordinate des Punktes
 * \param nx neue x-Koordinate des Punktes*/
void Point::setX(double nx) {
    x=nx;
}
/*! Verändert die y-Koordinate des Punktes
 * \param ny neue y-Koordinate des Punktes*/
void Point::setY(double ny) {
    y=ny;
}
/*! Gibt die x-Koordinate des Punktes zurück
 * \return x-Koordinate des Punktes */
double Point::getX() {
    return x;
}

/*! Gibt die y-Koordinate des Punktes zurück
 * \return y-Koordinate des Punktes */
double Point::getY() {
    return y;
}

/*! Setzt einen Punkt auf einen anderen Punkt
 * \param np Punkt dessen Koordinaten übernommen werden sollen
 * \return Punkt mit den Koordinaten vom Punkt np */
Point Point::operator = (Point* np) {
    Point* point = new Point(np->x,np->y);
    return *point;
}
