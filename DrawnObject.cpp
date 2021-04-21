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

void DrawnObject::addData(QPointF position, int time){
    int* t = new int;
    *t = time;
    QPointF* point = new QPointF(position.x(), position.y());
    positionInputPoints.append(point);
    timeInputPoints.append(t);
}

void DrawnObject::clean(){
    for(int i = 0; i < positionInputPoints.size(); i++){
        free(positionInputPoints[i]);
        free(timeInputPoints[i]);
    }
    vertices.clear();
    positionInputPoints.clear();
    timeInputPoints.clear();
}

DrawnObject::~DrawnObject(){
    clean();
    positionInputPoints.~QVector();
    timeInputPoints.~QVector();
    vertices.~QVector();
}

void DrawnObject::analyzeSpeed(){
    QPointF point(*positionInputPoints[0]);
    QPointF next(*positionInputPoints[1]);
    vertices.append(point); //first point drawn-- probably a vertex
    for (int i=1; i<positionInputPoints.size()-1; i++){
        point = QPointF(*positionInputPoints[i]);
        next = QPointF(*positionInputPoints[i+1]);
        QLineF segment(point, next);
        int dtime = abs(*timeInputPoints[i+1] - *timeInputPoints[i]);
        int speed = int(100*(segment.length())/dtime);
        if (speed<5 && vertices[vertices.size()-1] != point){
            vertices.append(point);
        }
    }
    vertices.append(next);
    //return analyzeLengths(eliminateColinear(vertices));
}

void DrawnObject::analyzeColinearity(){
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
    clean();
    vertices = returnVertices;
    returnVertices.clear();

    //return returnVertices;
}

void DrawnObject::analyzeDistances(){
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

    clean();
    vertices = returnVertices;
    returnVertices.clear();
    //return returnVertices;
}

QPointF DrawnObject::pointAverage(QVector<QPointF> points){
    if (points.size()==1){
        return points[0];
    }
    int x = 0;
    int y = 0;
    for (int i=0; i<points.size(); i++){
        x = x + int(points[i].x());
        y = y + int(points[i].y());
    }
    x = x/points.size();
    y = y/points.size();
    return QPointF(x, y);
}

int DrawnObject::maxLength(QVector<QPointF> vertices){
    int maxLength = 0;

    for (int i=1; i< vertices.size(); i++){
        int l = QLineF(vertices[i], vertices[i-1]).length();
        if (l>maxLength){
            maxLength = l;
        }
    }
    return maxLength;
}
