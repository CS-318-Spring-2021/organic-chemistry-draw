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
    vertices.append(next);
//    for (int i = 0; i<vertices.size(); i++){
//        printf("%i: (%i, %i)\n", i, int(vertices[i].x()), int(vertices[i].y()));
//    }

    //drawVerticesx(vertices, QPen(Qt::blue, 2.0));
    //vertices.clear();
    return analyzeLengths(eliminateColinear(vertices));
    //drawVerticesy(cleanedVertices, QPen(Qt::red, 2.0));
    //analyzeWithSlopes(15);
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
    //cleanedVertices.clear();
    vertices.clear();
    //vertices2.clear();
    vector.clear();
}

DrawnObject::~DrawnObject(){
    clean();
    vector.~QVector();
    vertices.~QVector();
    //vertices2.~QVector();
    //cleanedVertices.~QVector();
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

    lastLine = QLineF(vertices[0], vertices[1]);
    for (int i = 1;i<vertices.length()-1;i++){
        currentLine = QLineF(vertices[i], vertices[i+1]);
        if (abs(currentLine.angleTo(lastLine))>tolerance){
            returnVertices.append(vertices[i]);
        }
        lastLine = currentLine;
    }
    returnVertices.append(vertices[vertices.length()-1]);

    return returnVertices;
}

QVector<QPointF> DrawnObject::analyzeLengths(QVector<QPointF> vertices){
    QVector<int> lengths; //lengths at i is the distance between i-1 and i
    QVector<QPointF> returnVertices;
    int maxLength = 0;

    lengths.append(0);
    for (int i=1; i< vertices.size(); i++){
        int l = QLineF(vertices[i], vertices[i-1]).length();
        lengths.append(l);
        if (l>maxLength){
            maxLength = l;
        }
    }


    QVector<QPointF> clump;
    clump.append(vertices[0]);
    for (int i = 1; i<vertices.size(); i++){
        if (lengths[i] < maxLength/3){
            clump.append(vertices[i]);
        }
        else {
            returnVertices.append(pointAverage(clump));
            clump.clear();
            clump.append(vertices[i]);
        }
    }
    QPointF p = pointAverage(clump);
    clump.clear();
    int l = QLineF(returnVertices[0], p).length();
    if (l< maxLength/3){
        returnVertices.append(returnVertices[0]);
    }
    else {
        returnVertices.append(p);
    }
    return returnVertices;
}

QPointF DrawnObject::pointAverage(QVector<QPointF> points){
    if (points.size()==1){
        return points[0];
    }
    int x = 0;
    int y = 0;
    //printf("------\n");
    for (int i=0; i<points.size(); i++){
        //printf("  (%i, %i)\n", int(points[i].x()), int(points[i].y()));
        x = x + int(points[i].x());
        y = y + int(points[i].y());
    }
    x = x/points.size();
    y = y/points.size();
    //printf("==(%i, %i)\n", int(points[0].x()), int(points[0].y()));
    //printf("==(%i, %i)\n", x, y);
    //TODO: does this actually do a good / reasonable job computing the average?
    return QPointF(x, y);
}
