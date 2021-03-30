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
