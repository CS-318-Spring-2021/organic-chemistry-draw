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
    QVector<Molecule*> emptyMolecule;
    undoStackMolecule.append(emptyMolecule);
    QVector<DrawnObject*> emptyDrawnObject;
    undoStackDrawnObject.append(emptyDrawnObject);
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
                appending = -1; //needs to be commented out to make the commented code below do anything
            }else{
                Molecule *molecule = new Molecule(currentDrawnObject->vertices, bondLength);
                molecules.append(molecule);
            }

            QVector<Molecule*> deepCopy = makeMoleculesFreshCopy();
            undoStackMolecule.append(deepCopy);
            QVector<DrawnObject*> drawnCopy = makeDrawnObjectsFreshCopy();
            undoStackDrawnObject.append(drawnCopy);
        }
//        if(appending>-1){
//            QPointF lastPoint = molecules[appending]->atomSet.last()->atomPos;
//            for(int i = 0; i< molecules[appending]->atomSet.size()-1;i++){
//                QPointF currentPoint = molecules[appending]->atomSet[i]->atomPos;
//                QLineF line(lastPoint, currentPoint);
//                //printf("%f\n",line.length());
//                if(line.length()>.1){
//                    //printf("equal\n");
//                    //remove the last atom
//                    //put a pointer to i in the last bond
//                    molecules[appending]->atomSet.pop_back();
//                    molecules[appending]->bondSet.last()->atomSecond = molecules[appending]->atomSet[i];
//                    molecules[appending]->atomSet[i]->addBond(molecules[appending]->bondSet.last());
//                    break;
//                    //*molecules[appending]->atomSet[i] = &molecules[appending]->atomSet.last();
//                }
//            }
//        }
        //code can be used to check for an atom underneath another atom: see writeup
        currentDrawnObject->clean();
        drawExisting();
        if(bondLength == -1) bondLength = molecules[0]->bondLength;
    }else{
        freehandObject->addData(pos);
        DrawnObject *copy = new DrawnObject(*freehandObject);
        freeHandObjects.append(copy);
        QVector<DrawnObject*> deepCopy = makeDrawnObjectsFreshCopy();
        undoStackDrawnObject.append(deepCopy);
        QVector<Molecule*> moleculeCopy = makeMoleculesFreshCopy();
        undoStackMolecule.append(moleculeCopy);
        drawExisting();
        freehandObject->cleanFreehand();
    }
}

QVector<Molecule*> drawspace::makeMoleculesFreshCopy(){
    //Copies everything in molecule into undostack as pointers to new molecules
    //all pointers in arrays in molecule are not being copied, needs to be fixed to be accurate
    QVector<Molecule*> copy = molecules;
    QVector<Molecule*> deepCopy;
    for(int i =0; i < copy.length(); i++){
        Molecule *copiedMolecule= new Molecule(*copy[i]);
        deepCopy.append(copiedMolecule);
     }
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
        if(!recording) currentDrawnObject->addData(pos, evt->timestamp());
        else freehandObject->addData(pos);
        maybeAddSegment(pos);
    }
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
            mScene.addLine(QLineF(freeHandObjects[m]->positionInputPoints[i-1], freeHandObjects[m]->positionInputPoints[i]), QPen(Qt::gray, 2.0));
        }
    }
}
