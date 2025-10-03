// This is the .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own, as well as on the members.
// TODO: remove this comment header

#ifndef TOUR_H
#define TOUR_H

#include "Node.h"
#include "Point.h"

class Tour {
public:

    Tour(Point a, Point b, Point c, Point d);
    Tour();
    ~Tour();
    void show();
    void draw(QGraphicsScene* scene);
    int size();
    double distance();
    void insertNearest(Point p);
    void insertSmallest(Point p);
    void optimizeCrossingLines();
    bool linesCross(Point a, Point b, Point c, Point d);
    void reverseSegment(Node* start, Node* end);


private:

    Node* m_current = nullptr;
    Node* m_front = nullptr;

};

#endif // TOUR_H
