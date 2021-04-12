#include "DrawnObject.h"
#include <QWidget>
#include <stdio.h>
#include <QtMath>
#include <cfloat>

#include "drawspace.h"
#include "Molecule.h"

DrawnObject::DrawnObject(drawspace* view){
    this->view = view;
    clean();
}

void DrawnObject::addData(int x, int y, int time){
    int *data = new int[3];
    data[0] = x;
    data[1] = y;
    data[2] = time;
    vector.append(data);
}

QVector<QPointF> DrawnObject::analyze(){
    QPointF point(*(vector[0]), *(vector[0]+1));
    QPointF next(*(vector[1]), *(vector[1]+1));
    vertices.append(point); //first point drawn-- probably a vertex
    for (int i=1; i<vector.size()-1; i++){
        point = QPointF(*(vector[i]), *(vector[i]+1));
        next = QPointF(*(vector[i+1]), *(vector[i+1]+1));
        QLineF segment(point, next);
        int dtime = *(vector[i]+2) - *(vector[i-1]+2);
        int speed = int(100*(segment.length())/dtime);
        if (speed<5 && vertices[vertices.size()-1] != point){
            vertices.append(point);
        }
    }
    vertices.append(next); //last point drawn-- probably a vertex
//    for (int i = 0; i<vertices.size(); i++){
//        printf("%i: (%i, %i)\n", i, int(vertices[i].x()), int(vertices[i].y()));
//    }

    //drawVerticesx(vertices, QPen(Qt::blue, 2.0));
    QVector<QPointF> cleanedVertices = analyzeLengths(eliminateColinear(vertices)); /*
    for (int i = 0; i<cleanedVertices.size(); i++){
        printf("%i: (%i, %i)\n", i, int(cleanedVertices[i].x()), int(cleanedVertices[i].y()));
    }*/

    //vertices.clear();
    //vertices = cleanupVertices(cleanedVertices);
    drawVerticesy(cleanedVertices, QPen(Qt::red, 2.0));

    //analyzeWithSlopes(15);

    return cleanedVertices;
}
/*
void DrawnObject::analyzeWithSlopes(int gap) {

    bool haveAngle = false;

    float degrees = 0;
    float tolerence = 0.449;

    int maxLen = vector.length();
    int i = 0;

    vertices2.append(vector[0]);

    while (i < maxLen) {
        int x0 = *(vector[i]);
        int y0 = *(vector[i]+1);
        if ((i + gap) > maxLen-1) {
            gap = maxLen - 1 - i;
            if (gap < 6) { break; }
        }
        int x1 = *(vector[i+gap]);
        int y1 = *(vector[i+gap]+1);
        int x = (x0 - x1);
        int y = (y0 - y1);

        float currDegrees;
        if (x == 0) {
            if (y < 0) {
                currDegrees = -3.1415;
            } else {
                currDegrees = 0;
            }
        } else {
            currDegrees = atan(y/x);
        }
        if (!haveAngle || (abs(degrees - currDegrees) < tolerence)) {
            haveAngle = true;
            i = i + gap;
        } else {
            haveAngle = false;
            i = binarySearch(i, i+gap, degrees, tolerence);
        }
        degrees = currDegrees;
    }
    vertices2.append(vector[maxLen-1]);



    //printf("vertices length: %i\n", vertices.length());
    QVector<int*> cleanedVertices2 = cleanupVertices(vertices2);
    drawVerticesy(vertices2, QPen(Qt::red, 10.0));
    //printf("vertices2 length: %i\n", vertices.length());
    drawVerticesy(cleanedVertices2, QPen(Qt::blue, 2.0));
}

int DrawnObject::binarySearch(int start, int end, float degrees, float tolerence) {
    int difference = end - start;

    // base case, if only two or less left, we are pretty much done
    if (difference < 3) {
        vertices2.append(vector[end]);
        return end;
    }

    int halfway = end - int(difference/2);
    int x0 = *(vector[start]);
    int y0 = *(vector[start]+1);
    int x1 = *(vector[halfway]);
    int y1 = *(vector[halfway]+1);
    int x = (x0 - x1);
    int y = (y0 - y1);

    float currDegrees;
    if (x == 0) {
        if (y < 0) {
            currDegrees = -3.1415;
        } else {
            currDegrees = 0;
        }
    } else {
        currDegrees = atan(y/x);
    }
    if (abs(degrees - currDegrees) < tolerence) {
        return binarySearch(halfway, end, currDegrees, tolerence);
    } else {
        return binarySearch(start, halfway, degrees, tolerence);
    }
}
*/
void DrawnObject::clean(){
    for(int i = 0; i < vector.size(); i++){
        free(vector[i]);
    }
    cleanedVertices.clear();
    vertices.clear();
    //vertices2.clear();
    vector.clear();
}

DrawnObject::~DrawnObject(){
    clean();
    vector.~QVector();
    vertices.~QVector();
    //vertices2.~QVector();
    cleanedVertices.~QVector();
}

void DrawnObject::drawVerticesx(QVector<QPointF> vertices, QPen pen){
    for(int i = 0; i< vertices.length(); i++){
        QPointF firstPos = QPointF(vertices[i].x()-5, vertices[i].y());
        QPointF lastPos = QPointF(vertices[i].x()+10, vertices[i].y());
        view->replaceSegment(firstPos, lastPos, pen);
    }
}

void DrawnObject::drawVerticesy(QVector<QPointF> vertices, QPen pen){
    for(int i = 0; i< vertices.length(); i++){
        QPointF firstPos = QPointF(vertices[i].x(), vertices[i].y()-5);
        QPointF lastPos = QPointF(vertices[i].x(), vertices[i].y()+10);
        view->replaceSegment(firstPos, lastPos, pen);
    }
}

QVector<QPointF> DrawnObject::eliminateColinear(QVector<QPointF> vertices){
    QLineF currentLine;
    QLineF lastLine;
    int tolerance = 10;
    QVector<QPointF> returnVertices;
    returnVertices.append(vertices[0]);
    //printf("length: %i\n", vertices.length());

    lastLine = QLineF(vertices[0], vertices[1]);
    for (int i = 1;i<vertices.length()-1;i++){
        currentLine = QLineF(vertices[i], vertices[i+1]);
        if (abs(currentLine.angleTo(lastLine))>tolerance){
            returnVertices.append(vertices[i]);
        }
        lastLine = currentLine;

    }
    returnVertices.append(vertices[vertices.length()-1]);
    //printf("vtx length: %i\n", returnVertices.length());
    return returnVertices;
}

QVector<QPointF> DrawnObject::analyzeLengths(QVector<QPointF> vertices){
    QVector<QPointF> returnVertices;
    QVector<int> indices;
    indices.append(0);
    for (int i=1; i< vertices.size(); i++){
        int l = QLineF(vertices[i], vertices[i-1]).length();
        printf("%i to %i: %i\n", i-1, i, l);
        if (l>10){
            indices.append(l);

        }
        else {
            indices.append(l);
        }
    }
    int cot = 1;
    int x = vertices[0].x();
    int y = vertices[0].y();
    //returnVertices.append(vertices[0]);
    for (int i = 1; i<vertices.size(); i++){
        if (indices[i] < 10){ //if this is an inconsequential thing
            x = x + vertices[i].x();
            y = y + vertices[i].y();
            cot = cot + 1;
        }
        else {
            returnVertices.append(QPointF(x/cot, y/cot));
            cot = 1;
            x = vertices[i].x();
            y = vertices[i].y();
        }
    }
    returnVertices.append(QPointF(x/cot, y/cot));
    return returnVertices;
}
