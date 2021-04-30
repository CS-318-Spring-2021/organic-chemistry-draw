#include "mainwindow.h"

#include <QtWidgets>
#include <QDebug>
#include <stdio.h>

#include "drawspace.h"
#include "Molecule.h"
#include "qatom.h"
#include "qbond.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    QVector<Molecule*> molecules;
    QWidget *w = new QWidget();
    setCentralWidget(w);

    QHBoxLayout *mainLayout = new QHBoxLayout(w);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(table = new QTableWidget());
    rightLayout->addWidget(saveButton = new QPushButton("Save"));


    connect(saveButton, &QPushButton::clicked, this, &MainWindow::bSave);

    QVBoxLayout *taskBarLayout = new QVBoxLayout();
    taskBarLayout->addWidget(recordCheckBox = new QCheckBox("Record"));
    recordCheckBox->setCheckable(true);
    recordCheckBox->setChecked(true);

    connect(recordCheckBox, &QCheckBox::stateChanged, this, &MainWindow::bRecording);

    mainLayout->addLayout(taskBarLayout);
    mainLayout->addWidget(view = new drawspace(), 1);
    mainLayout->addLayout(rightLayout);

    currentDrawnObject = new DrawnObject(view);
    freehandObject = new DrawnObject(view);

    connect(view, &drawspace::mouseEvent, this, &MainWindow::onMouseEvent);

}

MainWindow::~MainWindow()
{

}

void MainWindow::bSave() {
    QString fName = QFileDialog::getSaveFileName(this, "Enter save filename", QString(), tr("Comma-separated values (*.csv)"));
    if (fName.isEmpty()) return;

    QFile fOut(fName);
    if (!fOut.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(this, "Error opening file", "Can't open selected file for output.");
        return;
    }

    QTextStream out(&fOut);
    out << "t,dt,x,y,speed,acceleration\n";
    for (int row = 0; row < table->rowCount(); ++row) {
        out << table->item(row, 0)->text() << "," << table->item(row, 1)->text() << "," << table->item(row, 2)->text() << "," << table->item(row, 3)->text();
        QString note = table->item(row, 4)->text();
        if (!note.isEmpty()) out << "," << note;
        out << table->item(row, 5)->text();
        out << "\n";
    }

    return;
}

void MainWindow::bRecording(){
    //TODO: whats goin on here
}

void MainWindow::onMouseEvent(int type, int when, QPointF pos) {
    //this should all be in view on the mouse press and release event

    static QString types = "PRM";
    static int when0 = -1;

    if (when0==-1) {
        when0 = when;
    }
    if(recordCheckBox->isChecked()){
        currentDrawnObject->addData(pos, when-when0);

        if (type == 0){
            /*void* item = view->getItem(pos);

            if (item != nullptr) {
                //figure out what it is we're hovering over
                void* classpointer = view->locationMap[item];
                try { //atom!
                    //if its an atom, know which atom we clicked
                    QAtom* qa = static_cast<QAtom*>(classpointer);
                    Atom* a = qa->atom;
                } catch(nullptr_t){
                    //if its a bond, know which bond we clicked
                    Bond* b = static_cast<QBond*>(classpointer)->bond;
                }
            }
            else */if(molecules.isEmpty()) {
                appending = -1;
            }else{
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


        }

        if (type == 1){
            QGraphicsItem* item = view->getItem(QPoint(int(pos.x()), int(pos.y()))); //

            if (item != nullptr) {
                printf("something happened?");
                //figure out what it is we're hovering over
                void* classpointer = view->locationMap[item];
                try { //atom!
                    //if its an atom, we are ENDING on an ATOM
                    QAtom* qa = static_cast<QAtom*>(classpointer);
                    Atom* a = qa->atom;
                } catch(nullptr_t){
                    //change the molecule's value at that point
                    Bond* b = static_cast<QBond*>(classpointer)->bond;
                    b->changeBond();
                    printf("changing some bond qualities B) \n");
                }
            }

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
            }

            currentDrawnObject->clean();
            view->locationMap.clear();
            drawExisting();
        }

    } else {
        freehandObject->addData(pos);
    }
}


void MainWindow::drawExisting(){
    for (int m=0; m < molecules.size(); m++){
        for (int i=0; i < (molecules[m]->atomSet.size()); i++){
            view->drawAtom(new QAtom(molecules[m]->atomSet[i], (molecules[m]->bondLength)/10));
        }
        for (int i = 0; i<(molecules[m]->bondSet.size()); i++){
            QBond* bond = new QBond(molecules[m]->bondSet[i]);
            bond->setZValue(-1);
            view->drawBond(bond);
        }
    }

    for (int f = 0; f<freehandObject->positionInputPoints.size(); f++){
        view->maybeAddSegment(*freehandObject->positionInputPoints[f]);
    }
}
