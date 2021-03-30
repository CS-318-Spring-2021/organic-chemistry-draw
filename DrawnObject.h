#ifndef DRAWNOBJECT_H
#define DRAWNOBJECT_H
#include <QWidget>

#include "drawspace.h"

class DrawnObject{
//    QVector<int*> vector;
    drawspace* view;
public:
    QVector<int*> vector;
    QVector<int*> vertices;
    QVector<float> speeds;
    DrawnObject(drawspace* view);
    void addData(int x, int y, int time);
    ~DrawnObject();
    void dealloc();
    void analyze();
    float speedCalc(int index);
    void analyzeWithSlopes();
    int* binarySearch(int start, int end);
};

#endif // DRAWNOBJECT_H
