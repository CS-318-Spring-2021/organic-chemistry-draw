#include "drawspace.h"
#include <QSurface>
#include <QDebug>
#include <QtWidgets>

#include "qatom.h"
#include "qbond.h"
#include "Molecule.h"
#include "DrawnObject.h"





drawspace::drawspace() {
    setScene(&mScene);
    setSceneRect(QRectF(-400.0, -300.0, 800.0, 600.0));
    setRenderHint(QPainter::Antialiasing);
    mouseIsDown = false;
    setMouseTracking(true);
    currentDrawnObject = new DrawnObject();
    freehandObject = new DrawnObject();
}

void drawspace::mousePressEvent(QMouseEvent *evt) {
    QGraphicsView::mousePressEvent(evt);
    mouseIsDown = true;
    QPointF pos = mapToScene(evt->pos());
    lastPos = pos;
    if(!recording){
        if(molecules.isEmpty()) {
            appending = -1;
        } else {
            float dist;
            float minDist = QLineF(pos, molecules[0]->atomSet[0]->atomPos).length();
            float minI = 0;
            for(int i = 0; i< molecules.size();i++){
                for(int j = 0; j< molecules[i]->atomSet.size();j++){
                    dist = QLineF(pos, molecules[i]->atomSet[j]->atomPos).length();
                    if(dist<minDist) {
                        minDist = dist;
                        minI = i;
                    }
                }
            }
            if(minDist < (molecules[minI]->bondLength)/10){
                appending = minI;
            }
        }

        lastPos = pos;
    }else{
        freehandObject->addData(pos);
    }
}


void drawspace::maybeAddSegment(const QPointF &pos) {
    if (lastPos!=pos) {
        mScene.addLine(QLineF(lastPos, pos), QPen(Qt::gray, 2.0));
        lastPos = pos;
    }
}

void drawspace::addFreehandSegment(QPointF pos) {
    if (lastPos!=pos) {
        mScene.addLine(QLineF(lastPos, pos), QPen(Qt::gray, 2.0));
        lastPos = pos;
    }
}


void drawspace::mouseReleaseEvent(QMouseEvent *evt) {
    mouseIsDown = false;
    QGraphicsView::mouseReleaseEvent(evt);
    QPointF pos = mapToScene(evt->pos());
    mScene.clear();
    if(!recording){
        if (currentDrawnObject->positionInputPoints.size()>5){

            currentDrawnObject->analyzeSpeed();
            currentDrawnObject->analyzeColinearity();
            currentDrawnObject->analyzeDistances();

            if(appending>-1){
                molecules[appending]->addNewVerts(currentDrawnObject->vertices);
                appending = -1;
            }else{
                Molecule *molecule = new Molecule(currentDrawnObject->vertices);
                molecules.append(molecule);
            }

            QVector<Molecule*> deepCopy = makeMoleculesFreshCopy();
            undoStackMolecule.append(deepCopy);

        }

        currentDrawnObject->clean();
        drawExisting();
    }else{
        freehandObject->addData(pos);
        DrawnObject *copy = new DrawnObject(*freehandObject);
        freeHandObjects.append(copy);
        QVector<DrawnObject*> deepCopy = makeDrawnObjectsFreshCopy();
        undoStackDrawnObject.append(deepCopy);
        drawExisting();
        freehandObject->cleanFreehand();

    }

}

QVector<Molecule*> drawspace::makeMoleculesFreshCopy(){
    //Copies everything in molecule into undostack as pointers to new molecules
    //all pointers are not being copied, needs to be fixed to be accurate
    QVector<Molecule*> copy = molecules;
    QVector<Molecule*> deepCopy;
    for(int i =0; i < copy.length(); i++){
        Molecule *copiedMolecule= new Molecule(*copy[i]);
        deepCopy.append(copiedMolecule);
     }

    //returns a coppied molecules at a differnt address
    return deepCopy;
}

QVector<DrawnObject*> drawspace::makeDrawnObjectsFreshCopy(){
    //Copies everything in freeHandObjects as pointers to new DrawnObjects
    QVector<DrawnObject*> copy = freeHandObjects;
    QVector<DrawnObject*> deepCopy;
    for(int i =0; i < copy.length(); i++){
        DrawnObject *copiedDrawnObject= new DrawnObject(*copy[i]);
        deepCopy.append(copiedDrawnObject);
     }

    //returns list of new DrawnObjects
    return deepCopy;
}

void drawspace::mouseMoveEvent(QMouseEvent *evt) {
    QGraphicsView::mouseMoveEvent(evt);
    if (mouseIsDown) {
        QPointF pos = mapToScene(evt->pos());
        if(!recording){
            currentDrawnObject->addData(pos, evt->timestamp());
        } else {
            freehandObject->addData(pos);
        }
        maybeAddSegment(pos);
    }
}

void drawspace::replaceSegment(const QPointF &firstPos, const QPointF &lastPos) {
    mScene.addLine(QLineF(firstPos, lastPos), QPen(Qt::black, 2.0));
}


void drawspace::drawExisting(){
    for (int m=0; m < molecules.size(); m++){
        for (int i=0; i < (molecules[m]->atomSet.size()); i++){
            mScene.addItem(new QAtom(molecules[m]->atomSet[i], (molecules[m]->bondLength)/10));
        }
        for (int i = 0; i<(molecules[m]->bondSet.size()); i++){
            QBond* bond = new QBond(molecules[m]->bondSet[i]);
            bond->setZValue(-1);
            mScene.addItem(bond);
        }
    }
    for (int m=0; m < freeHandObjects.size(); m++){
        for (int i = 1; i<freeHandObjects[m]->positionInputPoints.size(); i++){
            //printf("inputPoint: x: %f, y: %f\n", freeHandObjects[m]->positionInputPoints[i-1].x(),freeHandObjects[m]->positionInputPoints[i-1].y());
            mScene.addLine(QLineF(freeHandObjects[m]->positionInputPoints[i-1], freeHandObjects[m]->positionInputPoints[i]), QPen(Qt::gray, 2.0));
            //addFreehandSegment(*freeHandObjects[m]->positionInputPoints[i]);
        }
    }
}
