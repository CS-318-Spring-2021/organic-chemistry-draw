#include "DrawnObject.h"
#include <QWidget>
#include <stdio.h>
#include <QtMath>
#include <cfloat>

#include "drawspace.h"

DrawnObject::DrawnObject(drawspace* view){
    this->view = view;
    dealloc();
}

void DrawnObject::addData(int x, int y, int time){
    int *data = new int[3];
    data[0] = x;
    data[1] = y;
    data[2] = time;
    vector.append(data);
}

void DrawnObject::analyze(){
    float currentAngle;
    float lastAngle;
    float tolerance = 2.0;
    QLineF line;
    vertices.append(vector[0]);
    if (!(vector.size()>8)){
        return;
    }
    bool decreasing = false;
    speeds.append(speedCalc(6));
    for (int i = 7;i<vector.size()-6;i++){
        //calculate distance between 2 points
        float speed = speedCalc(i);
        speeds.append(speed);
        if (speeds[i-7]<speeds[i-6] && decreasing == true){
            decreasing = false;
            //printf("%i\n", i-7);
            vertices.append(vector[i-7]);
            //printf("%f\n", speed);
        }else if (speeds[i-7]>speeds[i-6] && decreasing == false){
            decreasing = true;
        }
    }

    vertices.append(vector[vector.size()-1]);
    //view->clear
    drawVertices(vertices, QPen(Qt::blue, 10.0));
    cleanedVertices.append(vertices[0]);
    printf("vertices length: %i\n", vertices.length());
    QPointF firstPos = QPointF(*(vertices[0]), *(vertices[0]+1));
    QPointF lastPos = QPointF(*(vertices[1]), *(vertices[1]+1));
    line = QLineF(firstPos, lastPos);
    currentAngle = line.angle();
    for (int i = 1;i<vertices.length()-2;i++){
        firstPos = QPointF(*(vertices[i]), *(vertices[i]+1));
        lastPos = QPointF(*(vertices[i+1]), *(vertices[i+1]+1));
        lastAngle = currentAngle;
        line = QLineF(firstPos, lastPos);
        currentAngle = line.angle();
        //printf("%f\n",abs(lastAngle-currentAngle));
        if(abs(lastAngle-currentAngle) > tolerance){
            cleanedVertices.append(vertices[i]);
        }
    }
    cleanedVertices.append(vertices[vertices.length()-1]);
    drawVertices(cleanedVertices, QPen(Qt::red, 5.0));

    for (int i = 0;i<cleanedVertices.length()-1;i++){

        QPointF firstPos = QPointF(*(cleanedVertices[i]), *(cleanedVertices[i]+1));
        QPointF lastPos = QPointF(*(cleanedVertices[i+1]), *(cleanedVertices[i+1]+1));
        view->replaceSegment(firstPos, lastPos);
    }
}

void DrawnObject::analyzeWithSlopes() {

    bool haveAngle = false;

    float degrees = 0;
    float tolerence = 0.449;

    int maxLen = vector.length();
    int i = 0;
    int gap = 15;

    while (i < maxLen - 1) {
        int x0 = *(vector[i]);
        int y0 = *(vector[i]+1);
        if ((i + gap) > maxLen - 1) {
            gap = maxLen - 1 - i;
            if (gap < 6) { break; }
        }
        int x1 = *(vector[i+gap]);
        int y1 = *(vector[i+gap]+1);
        int x = (x0 - x1);
        int y = (y0 - y1);

        float currDegrees;
        if (x == 0) {
            currDegrees = atan(FLT_MAX);
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

    for (int j = 0; j < vertices.length(); j++) {
        printf("x: %i ", *(vertices[j]));
        printf("y: %i\n", *(vertices[j]+1));
    }
}

int DrawnObject::binarySearch(int start, int end, float degrees, float tolerence) {
    int difference = end - start;

    // base case, if only two or less left, we are pretty much done
    if (difference < 3) {
        int* point = new int[2];
        point[0] = *(vector[end]);
        point[1] = *(vector[end] + 1);
        vertices.append(point);
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
        currDegrees = atan(FLT_MAX);
    } else {
        currDegrees = atan(y/x);
    }
    if (abs(degrees - currDegrees) < tolerence) {
        return binarySearch(halfway, end, currDegrees, tolerence);
    } else {
        return binarySearch(start, halfway, degrees, tolerence);
    }
}


float DrawnObject::speedCalc(int i){
    float returnSpeed = 0;
    for(int j = 1; j<6; j++){
        float distance = qSqrt(pow(*(vector[i+j])-*(vector[i-j]),2)+pow(*(vector[i+j]+1)-*(vector[i-j]+1),2));
        float speed = distance / (*(vector[i+j]+2) - *(vector[i-j]+2));
        returnSpeed+=speed;
    }

    return returnSpeed/5;
}

void DrawnObject::dealloc(){
    for(int i = 0; i < vector.size(); i++){
        free(vector[i]);
    }
    cleanedVertices.clear();
    vertices.clear();
    vector.clear();
}

DrawnObject::~DrawnObject(){
    dealloc();
    vector.~QVector();
}

void DrawnObject::drawVertices(QVector<int*> vertices, QPen pen){
    for(int i = 0; i< vertices.length(); i++){
        QPointF firstPos = QPointF(*(vertices[i]), *(vertices[i]+1));
        QPointF lastPos = QPointF(firstPos.x()+1, firstPos.y()+1);
        view->replaceSegment(firstPos, lastPos, pen);
    }
}
