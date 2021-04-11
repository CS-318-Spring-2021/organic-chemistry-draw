#include "DrawnObject.h"
#include <QWidget>
#include <stdio.h>
#include <QtMath>
#include <cfloat>

#include "drawspace.h"

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

void DrawnObject::analyze(){
    vertices.append(vector[0]); //first point drawn-- probably a vertex

    QLineF segment(*(vector[0]), *(vector[0]+1), *(vector[1]), *(vector[1]+1));
    int dtime = *(vector[1]+2) - *(vector[0]+2);
    for (int i=1; i<vector.size()-1; i++){
        segment = QLineF(*(vector[i]), *(vector[i]+1), *(vector[i+1]), *(vector[i+1]+1));
        dtime = *(vector[i]+2) - *(vector[i-1]+2);
        int speed = int(100*(segment.length())/dtime);
        if (speed<5){
            vertices.append(vector[i]);
        }
    }
    vertices.append(vector[vector.size()-1]);
    //drawVerticesx(vertices, QPen(Qt::blue, 2.0));
    QVector<int*> cleanedVertices = cleanupVertices(vertices);
    //vertices.clear();
    //vertices = cleanupVertices(cleanedVertices);
    drawVerticesy(cleanedVertices, QPen(Qt::red, 2.0));

    //analyzeWithSlopes(15);

    for (int i = 0;i<cleanedVertices.length()-1;i++){

        QPointF firstPos = QPointF(*(cleanedVertices[i]), *(cleanedVertices[i]+1));
        QPointF lastPos = QPointF(*(cleanedVertices[i+1]), *(cleanedVertices[i+1]+1));
        view->replaceSegment(firstPos, lastPos);
    }
}

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

void DrawnObject::clean(){
    for(int i = 0; i < vector.size(); i++){
        free(vector[i]);
    }
    cleanedVertices.clear();
    vertices.clear();
    vertices2.clear();
    vector.clear();
}

DrawnObject::~DrawnObject(){
    clean();
    vector.~QVector();
    vertices.~QVector();
    vertices2.~QVector();
    cleanedVertices.~QVector();
}

void DrawnObject::drawVerticesx(QVector<int*> vertices, QPen pen){
    for(int i = 0; i< vertices.length(); i++){
        QPointF firstPos = QPointF(*(vertices[i])-5, *(vertices[i]+1));
        QPointF lastPos = QPointF(firstPos.x()+10, firstPos.y());
        view->replaceSegment(firstPos, lastPos, pen);
    }
}

void DrawnObject::drawVerticesy(QVector<int*> vertices, QPen pen){
    for(int i = 0; i< vertices.length(); i++){
        QPointF firstPos = QPointF(*(vertices[i]), *(vertices[i]+1)-5);
        QPointF lastPos = QPointF(firstPos.x(), firstPos.y()+10);
        view->replaceSegment(firstPos, lastPos, pen);
    }
}

QVector<int*> DrawnObject::cleanupVertices(QVector<int*> vertices){
    int currentAngle;
    int lastAngle;
    int tolerance = 10;
    QLineF line;
    QVector<int*> returnVertices;
    returnVertices.append(vertices[0]);
    //printf("length: %i\n", vertices.length());
    QPointF firstPos = QPointF(*(vertices[0]), *(vertices[0]+1));
    QPointF lastPos = QPointF(*(vertices[1]), *(vertices[1]+1));
    line = QLineF(firstPos, lastPos);
    currentAngle = int(line.angle());
    for (int i = 1;i<vertices.length()-1;i++){
        firstPos = QPointF(*(vertices[i]), *(vertices[i]+1));
        lastPos = QPointF(*(vertices[i+1]), *(vertices[i+1]+1));
        lastAngle = currentAngle;
        line = QLineF(firstPos, lastPos);
        currentAngle = int(line.angle());
        if(diff(lastAngle,currentAngle) > tolerance){
            returnVertices.append(vertices[i]);
        }
    }
    returnVertices.append(vertices[vertices.length()-1]);
    printf("vtx length: %i\n", returnVertices.length());
    return returnVertices;
}

int DrawnObject::diff(int a, int b){
    //a and b are both on a scale of 0 to 360.
    //if a is 0 and b is 350, return 10.
    //if a is 350 and b is 0, return 10
    //if a is 200 and b is 300, return 100
    //is this modulo?
    //oh crap! this totally is mod!
    //so its? subtract them, mod 360?

    int t = (a-b)%360;
    return abs(t);
    //return 0;
}
