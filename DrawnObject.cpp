#include "DrawnObject.h"
#include <QWidget>
#include <stdio.h>

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
    vertices.append(vector[vector.size()-1]);
    //view->clear
    for (int i = 0;i<vertices.length()-1;i++){
        QPointF firstPos = QPointF(*(vertices[i]), *(vertices[i]+1));
        QPointF lastPos = QPointF(*(vertices[i+1]), *(vertices[i+1]+1));
        view->replaceSegment(firstPos, lastPos);
    }

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
