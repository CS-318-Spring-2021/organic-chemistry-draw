#include "DrawnObject.h"
#include <QWidget>
#include <stdio.h>
#include <QtMath>
#include <math.h>

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
    analyzeWithSlopes(15);

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
    for (int i = 0;i<vertices.length()-1;i++){
        QPointF firstPos = QPointF(*(vertices[i]), *(vertices[i]+1));
        QPointF lastPos = QPointF(*(vertices[i+1]), *(vertices[i+1]+1));
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
    for (int j = 0; j < vertices2.length(); j++) {
        printf("x: %i ", *(vertices2[j]));
        printf("y: %i\n", *(vertices2[j]+1));
    }
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


float DrawnObject::speedCalc(int i){
    float returnSpeed = 0;
    for(int j = 1; j<6; j++){
        float distance = qSqrt(pow(*(vector[i+j])-*(vector[i-j]),2)+pow(*(vector[i+j]+1)-*(vector[i-j]+1),2));
        float speed = distance / (*(vector[i+j]+2) - *(vector[i-j]+2));
        returnSpeed+=speed;
    }

    return returnSpeed/5;
}

void DrawnObject::clean(){
    for(int i = 0; i < vector.size(); i++){
        free(vector[i]);
    }
    vertices.clear();
    vertices2.clear();
    vector.clear();
}

DrawnObject::~DrawnObject(){
    clean();
    vector.~QVector();
}
