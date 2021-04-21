#include "mainwindow.h"

#include <QtWidgets>
#include <QDebug>
#include <stdio.h>

#include "drawspace.h"
#include "Molecule.h"

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

    table->setColumnCount(6);
    table->setColumnWidth(0, 50);
    table->setColumnWidth(1, 50);
    table->setColumnWidth(2, 50);
    table->setColumnWidth(3, 50);
    table->setColumnWidth(4, 70);
    table->setColumnWidth(5, 100);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setMinimumWidth(350);
    table->verticalHeader()->setVisible(false);

    table->setHorizontalHeaderLabels(QStringList() << "t" << "dt" << "x" << "y" << "speed" << "acceleration");

    currentDrawnObject = new DrawnObject(view);

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
    if(!recordCheckBox->isChecked()){
        return;
    }

    static QString types = "PRM";
    static int when0 = -1;

    if (when0==-1) when0 = when;

    currentDrawnObject->addData(pos, when-when0);
    if (type == 1){

        currentDrawnObject->analyzeSpeed();
        currentDrawnObject->analyzeDistances();
        currentDrawnObject->analyzeColinearity();

        if (molecules.isEmpty()){
            Molecule *molecule = new Molecule(currentDrawnObject->vertices);
            molecules.append(molecule);
        } else {
            //figure out WHICH molecule to add it to
            int i = 0;
            molecules[i]->addNewVerts(currentDrawnObject->vertices);
        }

        currentDrawnObject->clean();
        for (int i = 0; i<(molecules[0]->bondSet.size()); i++){
            //create line segment representing bond object
            Bond *bond = molecules[0]->bondSet[i];

            QPointF a = bond->atomFirst->getAtomPos();
            QPointF b = bond->atomSecond->getAtomPos();

            int quantity = bond->quantity; //0, 1, 2
            int quality = bond->quality; //0, 1, 2

            //00, 01, 02, or 10, 20 //TODO: i think there is a more efficient way to do this nesting of ifs 😎

            if (quality==0 && quantity==0){
                view->replaceSegment(a, b);
            }
            else if (quantity>0){
                view->drawMultipleBond(a, b, quantity);
            }
            else if (quality>0){
                view->drawDimensionalBond(a, b, quality);

            }
        }


    }

    int row;
    table->setRowCount((row = table->rowCount())+1);

    table->setItem(row, 0, new QTableWidgetItem(QString::number(when-when0)));//types.mid(type, 1))); //time is stored in column 0
    //table->setItem(row, 1, new QTableWidgetItem(QString::number(when-when0)));
    table->setItem(row, 2, new QTableWidgetItem(QString::number(pos.x())));
    table->setItem(row, 3, new QTableWidgetItem(QString::number(pos.y())));
    if (row>0){
        QLineF segment(pos.x(), pos.y(),table->item(row-1, 2)->text().toDouble(), table->item(row-1, 3)->text().toDouble());
        int dtime = table->item(row, 0)->text().toInt()-(table->item(row-1,0)->text().toInt());
        table->setItem(row, 1, new QTableWidgetItem(QString::number(dtime)));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(int(100*(segment.length())/dtime))));
        if (row>1){
            //what is the distance right now? what was the distance before this?
            double prevdist = table->item(row-1, 4)->text().toDouble();
            double dist = table->item(row, 4)->text().toDouble();
            table->setItem(row, 5, new QTableWidgetItem(QString::number(dist-prevdist)));
        }
        /*if (table->item(row,4)->text().toInt()<1){
            printf("(%i, %i)\n", int(pos.x()), int(pos.y()));
        }*/
    }
}
