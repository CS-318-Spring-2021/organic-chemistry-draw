#include "DrawnObject.h"
#include <QWidget>
#include <stdio.h>
#include <QtMath>
#include <cfloat>

#include "drawspace.h"
#include "Molecule.h"

DrawnObject::DrawnObject(){
    clean();
}

void DrawnObject::addData(QPointF position, int time){
    int t = time;
    QPointF point(position.x(), position.y());
    positionInputPoints.append(point);
    timeInputPoints.append(t);
}

void DrawnObject::addData(QPointF position){
    QPointF point(position.x(), position.y());
    positionInputPoints.append(point);
}

void DrawnObject::clean(){
    vertices.clear();
    positionInputPoints.clear();
    timeInputPoints.clear();
}

void DrawnObject::cleanFreehand(){
    vertices.clear();
    positionInputPoints.clear();
}

DrawnObject::~DrawnObject(){
    clean();
    positionInputPoints.~QVector();
    timeInputPoints.~QVector();
    vertices.~QVector();
}

void DrawnObject::analyzeSpeed(){
    QPointF point(positionInputPoints[0]);
    QPointF next(positionInputPoints[1]);
    vertices.append(point); //first point drawn-- probably a vertex
    for (int i=1; i<positionInputPoints.size()-1; i++){
        point = QPointF(positionInputPoints[i]);
        next = QPointF(positionInputPoints[i+1]);
        QLineF segment(point, next);
        int dtime = abs(timeInputPoints[i+1] - timeInputPoints[i]);
        int speed = int(100*(segment.length())/dtime);
        if (speed<5 && vertices[vertices.size()-1] != point){
            vertices.append(point);
        }
    }
    vertices.append(next);
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
}

void DrawnObject::analyzeDistances(){
    QVector<int> lengths; //lengths at i is the distance between i-1 and i
    QVector<QPointF> returnVertices;
    int maxLength = 0;

    lengths.append(0);
    for (int i=1; i< vertices.size(); i++){
        int l = QLineF(vertices[i], vertices[i-1]).length();
        lengths.append(l);
        if (l>maxLength) maxLength = l;
    }

    QVector<QPointF> pointGrouping;
    pointGrouping.append(vertices[0]);
    for (int i = 1; i<vertices.size(); i++){
        if (lengths[i] < maxLength/3) pointGrouping.append(vertices[i]);
        else {
            returnVertices.append(pointAverage(pointGrouping));
            pointGrouping.clear();
            pointGrouping.append(vertices[i]);
        }
    }
    QPointF p = pointAverage(pointGrouping);
    pointGrouping.clear();
    int l = QLineF(returnVertices[0], p).length();
    if (l< maxLength/3) returnVertices.append(returnVertices[0]);
    else returnVertices.append(p);
    clean();
    vertices = returnVertices;
    returnVertices.clear();
}

QPointF DrawnObject::pointAverage(QVector<QPointF> points){
    if (points.size()==1) return points[0];
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
        int line = QLineF(vertices[i], vertices[i-1]).length();
        if (line>maxLength) maxLength = line;
    }
    return maxLength;
}
