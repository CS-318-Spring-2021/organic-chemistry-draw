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
    //drawVerticesx(vertices, QPen(Qt::blue, 10.0));
    QVector<int*> cleanedVertices = cleanupVertices(vertices);
    drawVerticesx(cleanedVertices, QPen(Qt::red, 2.0));

    analyzeWithSlopes();

//    for (int i = 0;i<cleanedVertices.length()-1;i++){

//        QPointF firstPos = QPointF(*(cleanedVertices[i]), *(cleanedVertices[i]+1));
//        QPointF lastPos = QPointF(*(cleanedVertices[i+1]), *(cleanedVertices[i+1]+1));
//        view->replaceSegment(firstPos, lastPos);
//    }
}

void DrawnObject::analyzeWithSlopes() {

    bool haveAngle = false;

    float radians = 0;
    float tolerence = 0.2617994;

    int gap = 8;

    int maxLen = vector.length();
    printf("maxLen = %i\n", maxLen);
    int i = 0;
    vertices2.append(vector[0]);

    while (i < maxLen) {
        int x0 = *(vector[i]);
        int y0 = *(vector[i]+1);
        if ((i + gap) > maxLen-1) {
            gap = maxLen - 1 - i;
            if (gap < 3) { break; }
        }
        int x1 = *(vector[i+gap]);
        int y1 = *(vector[i+gap]+1);
        int xDiff = (x0 - x1);
        int yDiff = (y0 - y1);

        float currRadians;
        if (xDiff == 0) {
            if (yDiff < 0) {
                currRadians = 1.570796;
            } else {
                currRadians = 4.7123896;
            }
        } else {
            currRadians = atan(yDiff/xDiff);
        }
        if (!haveAngle) {
            haveAngle = true;
            i = i + gap;
            radians = currRadians;
        } else if (abs(radians - currRadians - tolerence) < tolerence) {
            i = i + gap;
        } else {
            i = binarySearch(i, i+gap, radians, tolerence);
            vertices2.append(vector[i]);
            haveAngle = false;
        }
    }
    vertices2.append(vector[maxLen-1]);

    QVector<int*> cleanedVertices2;

    int prevCount = vertices2.length();
    cleanedVertices2 = cleanupVertices(vertices2);

    while (prevCount > cleanedVertices2.length()) {
        prevCount = cleanedVertices2.length();
        cleanedVertices2 = cleanupVertices(cleanupVertices(vertices2));
    }
    //drawVertices(vertices2, QPen(Qt::blue, 2.0));
    printf("vertices2 length: %i\n", vertices.length());
    drawVertices(cleanedVertices2, QPen(Qt::blue, 2.0));
}

int DrawnObject::binarySearch(int start, int end, float radians, float tolerence) {
    int difference = end - start;

    // base case, if only two or less left, we are pretty much done
    if (difference < 3) {
        return start;
    }

    int halfway = end - int(difference/2);
    int x0 = *(vector[start]);
    int y0 = *(vector[start]+1);
    int x1 = *(vector[halfway]);
    int y1 = *(vector[halfway]+1);
    int xDiff = (x0 - x1);
    int yDiff = (y0 - y1);

    float currRadians;
    if (xDiff == 0) {
        if (yDiff < 0) {
            currRadians = 1.570796;
        } else {
            currRadians = 4.7123896;
        }
    } else {
        currRadians = atan(yDiff/xDiff);
    }
    if (abs(radians - currRadians) < tolerence) {
        return binarySearch(halfway, end, radians, tolerence);
    } else {
        return binarySearch(start, halfway, radians, tolerence);
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

void DrawnObject::drawVertices(QVector<int*> vertices, QPen pen){
    for(int i = 0; i< vertices.length(); i++){
        QPointF firstPos = QPointF(*(vertices[i]), *(vertices[i]+1)-5);
        QPointF lastPos = QPointF(firstPos.x(), firstPos.y()+10);
        view->replaceSegment(firstPos, lastPos, pen);
    }
}

QVector<int*> DrawnObject::cleanupVertices(QVector<int*> vertices){
    float currentAngle;
    float lastAngle;
    float tolerance = 20.0;
    QLineF line;
    QVector<int*> returnVertices;
    returnVertices.append(vertices[0]);
    printf("cleaned length: %i\n", vertices.length());
    QPointF firstPos = QPointF(*(vertices[0]), *(vertices[0]+1));
    QPointF lastPos = QPointF(*(vertices[1]), *(vertices[1]+1));
    line = QLineF(firstPos, lastPos);
    currentAngle = line.angle();
    for (int i = 1;i<vertices.length()-1;i++){
        firstPos = QPointF(*(vertices[i]), *(vertices[i]+1));
        lastPos = QPointF(*(vertices[i+1]), *(vertices[i+1]+1));
        lastAngle = currentAngle;
        line = QLineF(firstPos, lastPos);
        currentAngle = line.angle();
        //printf("%f\n",abs(lastAngle-currentAngle));
        if(abs(lastAngle-currentAngle) > tolerance){
            returnVertices.append(vertices[i]);
        }
    }
    returnVertices.append(vertices[vertices.length()-1]);
    return returnVertices;
}
