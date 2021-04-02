#include "DrawnObject.h"
#include <QWidget>
#include <stdio.h>
#include <QtMath>

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
            printf("%f\n", speed);
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

void DrawnObject::analyzeWithSlopes() {
    int maxLen = vector.length();
    bool havePrevSlope = false;
    float slope = 0;
    int i = 0;
    int gap = 15;

    while (i < maxLen - 1) {
        int x0 = *(vector[i]);
        int y0 = *(vector[i]+1);
        if ((i + gap) < maxLen) {
            int x1 = *(vector[i+gap]);
            int y1 = *(vector[i+gap]+1);
            float nextSlope = (y0 - y1) / (x0 - x1);
            // what to do now?
        } else {
            // what to do now?

        }
    }

    for (int j = 0; j < vertices.length(); j++) {
        printf("x: %i ", *(vertices[j]));
        printf("y: %i\n", *(vertices[j]+1));
    }
}

int DrawnObject::binarySearch(int start, int end, float slope) {
    int difference = end - start;

    // base case, if only two or less left, we are pretty much done
    if (difference < 3) {
        int* point = new int[2];
        point[0] = *(vector[start]);
        point[1] = *(vector[start] + 1);
        vertices.append(point);
        return end;
    }

    int halfway = end - int(difference/2);
    int x0 = *(vector[start]);
    int y0 = *(vector[start]+1);
    int x1 = *(vector[halfway]);
    int y1 = *(vector[halfway]+1);
    float currentSlope = (y0 - y1) / (x0 - x1);
    if (abs(slope - currentSlope) < 1.5) {
        return binarySearch(halfway, end, slope);
    } else {
        return binarySearch(start, halfway, slope);
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
    vertices.clear();
    vector.clear();
}

DrawnObject::~DrawnObject(){
    dealloc();
    vector.~QVector();
}
