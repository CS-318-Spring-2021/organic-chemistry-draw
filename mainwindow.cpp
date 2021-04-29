#include "mainwindow.h"

#include <QtWidgets>
#include <QDebug>
#include <stdio.h>

#include "drawspace.h"
#include "Molecule.h"
#include "qatom.h"
#include "qbond.h"


//save , label pages, keep in file system, save as a .png or .pdf? or interactive thing?
//      students download Molecule class as a .txt or .xml file is possible,
//      read into application-- if students have application they can open / use / edit
//      easy way to jot things down.

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

    connect(table, &QTableWidget::currentCellChanged, this, &MainWindow::updateCrosshairs);
}

MainWindow::~MainWindow()
{

}

void MainWindow::updateCrosshairs(int curRow, int, int, int) {
    view->setCrosshairsVisible(true);
    view->setCrosshairsPos(QPointF(table->item(curRow, 2)->text().toDouble(), table->item(curRow, 3)->text().toDouble()));
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

}

void MainWindow::onMouseEvent(int type, int when, QPointF pos) {


    static QString types = "PRM";
    static int when0 = -1;

    if (when0==-1) when0 = when;
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
            void* item = view->getItem(pos);

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
