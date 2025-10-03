// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"

Tour::Tour(Point a, Point b, Point c, Point d)
{
    Node* na = new Node(a);
    Node* nb = new Node(b);
    Node* nc = new Node(c);
    Node* nd = new Node(d);

    na -> next = nb;
    nb -> next = nc;
    nc -> next = nd;
    nd -> next = na;

    m_front = na;

}

Tour::Tour()
{
    m_front = nullptr;
}

Tour::~Tour()
{
    if(m_front != nullptr){
        m_current = m_front->next;
        Node* first = m_current->next;
        while(first != m_front)
        {
            delete m_current;
            m_current = first;
            if(first != m_front)
            {
                first = m_current->next;
            }
        }
        delete m_current;
        delete m_front;
        m_front = nullptr;
        m_current = nullptr;
    }
}

void Tour::show()
{

    Node* current = m_front;
    bool lastNode = false;

    while(!lastNode){
        if(current->next == m_front){
            lastNode = true;
        }
        cout << current->point << endl;
        current = current->next;
    }

}

void Tour::draw(QGraphicsScene *scene)
{
    Node* current = m_front;
    bool lastNode = false;

    while(!lastNode){
        current->point.drawTo(current->next->point, scene);
        if(current->next == m_front){
            lastNode = true;
        }
        current = current->next;
    }
}

int Tour::size()
{
    Node* current = m_front;
    bool lastNode = false;
    int nodeCount = 0;

    while(!lastNode){
        if(current->next == m_front){
            lastNode = true;
        }
        ++nodeCount;
        current = current->next;
    }
    return nodeCount;
}

double Tour::distance()
{
    double distance = 0.0;
    Node* current = m_front;
    bool lastNode = false;

    while (!lastNode) {
        distance += current->point.distanceTo(current->next->point);

        if (current->next == m_front) {
            lastNode = true;
        }

        current = current->next;
    }

    return distance;
}


void Tour::insertNearest(Point p)
{
    // TODO: write this member
    if(m_front == nullptr){
        Node* n = new Node(p);
        n->next = n;
        m_front = n;
    }
    else if (m_front->next == m_front) {
        // Only one node in the tour, just add second one
        Node* n = new Node(p, m_front);
        m_front->next = n;
    }
    else{
        Node* current = m_front;
        bool lastNode = false;
        double minDistance = std::numeric_limits<double>::max();
        Node* bestNode = nullptr;

        while(!lastNode){
            if(current->next == m_front){
                lastNode = true;
            }
            // calculate where p should be inserted
            double distance = current->point.distanceTo(p);
            if(distance < minDistance){
                bestNode = current;
                minDistance = distance;
            }
            current = current->next;
        }
        //add nullptr check?
        Node* rest = bestNode->next;
        bestNode->next = new Node(p,rest);
    }

}

void Tour::insertSmallest(Point p)
{
    if(m_front == nullptr){
        Node* n = new Node(p);
        n->next = n;
        m_front = n;
    }
    else if (m_front->next == m_front) {
        // Only one node in the tour, just add second one
        Node* n = new Node(p, m_front);
        m_front->next = n;
    }
    else{
        Node* current = m_front;
        bool lastNode = false;

        double bestIncrease = std::numeric_limits<double>::infinity();
        Node* bestNode = nullptr;

        while(!lastNode){
            if(current->next == m_front){
                lastNode = true;
            }
            Node* nextNode = current->next;
            double oldDist = current->point.distanceTo(nextNode->point);
            double newDist = current->point.distanceTo(p) + p.distanceTo(nextNode->point);
            double increase = newDist - oldDist;

            if (increase < bestIncrease) {
                bestIncrease = increase;
                bestNode = current;
            }

            current = current->next;
        }
        //add nullptr check?
        Node* rest = bestNode->next;
        bestNode->next = new Node(p,rest);
    }
}
void Tour::optimizeCrossingLines()
{
    if (m_front == nullptr || m_front->next == m_front || m_front->next->next == m_front) {
        return; // Need at least 4 nodes for crossings to exist
    }
    bool improve = true;

    //Optimizeuntil cant improve
    while(improve){
        improve = false;

        Node* A = m_front;
        do {
            Node* B = A->next;
            Node* C = B->next;

            // Walk through edges (C,D) that are not adjacent to (A,B)
            while(C->next != A){
                Node* D = C->next;
                // Skip if they share endpoints
                if (A != C && A != D && B != C && B != D) {
                    if (linesCross(A->point, B->point, C->point, D->point)) {
                        // Found a crossing: apply 2-opt swap
                        reverseSegment(B, C);
                        A->next = C;
                        B->next = D;

                        improve = true;
                        break; // restart search
                    }
                }

                C = C->next;
            }
            if(improve) break;
            A = A->next;


        }while (A != m_front);
    }
}

bool Tour::linesCross(Point a, Point b, Point c, Point d){

    //calculate which side of a line a point is on
    auto orientation = [](Point p1, Point p2, Point p){
        double value = (p2.x - p1.x) * (p.y - p1.y) - (p2.y - p1.y) * (p.x - p1.x);
        return value;
    };
    // Get orientation relative to a point
    double o1 = orientation(c, d, a);
    double o2 = orientation(c, d, b);
    double o3 = orientation(a, b, c);
    double o4 = orientation(a, b, d);

    // Check if they're on opposite sides
    // We need both conditions to be true for a crossing
    bool cross1 = (o1 > 0 && o2 < 0) || (o1 < 0 && o2 > 0);
    bool cross2 = (o3 > 0 && o4 < 0) || (o3 < 0 && o4 > 0);

    return cross1 && cross2;
}

// Reverse the order of nodes from start to end)
void Tour::reverseSegment(Node* start, Node* end) {
    Node* prev = end->next;  // The node right after 'end'
    Node* curr = start;      // Begin reversing from 'start'
    Node* next = nullptr;

    while (curr != end->next) {   // Loop until we reach node after 'end'
        next = curr->next;       // Remember next node
        curr->next = prev;       // Reverse the link
        prev = curr;             // Move 'prev' one step forward
        curr = next;             // Advance current
    }
}

