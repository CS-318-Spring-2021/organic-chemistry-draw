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
    QWidget *w = new QWidget();
    setCentralWidget(w);

    QHBoxLayout *mainLayout = new QHBoxLayout(w);

    QVBoxLayout *rightLayout = new QVBoxLayout();
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
    bRecording();


}

MainWindow::~MainWindow()
{

}

void MainWindow::bSave() {
    QString fName = QFileDialog::getSaveFileName(this, "Enter save filename", QString(), tr("Pee Enn Gee (*.png)"));
    if (fName.isEmpty()) return;

    QFile fOut(fName);
    if (!fOut.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(this, "Error opening file", "Can't open selected file for output.");
        return;
    }

    QPixmap pixMap = view->grab(view->sceneRect().toRect());
    pixMap.save(fName);

//    QTextStream out(&fOut);
//    out << "t,dt,x,y,speed,acceleration\n";
//    /*
//    for (int row = 0; row < table->rowCount(); ++row) {
//        out << table->item(row, 0)->text() << "," << table->item(row, 1)->text() << "," << table->item(row, 2)->text() << "," << table->item(row, 3)->text();
//        QString note = table->item(row, 4)->text();
//        if (!note.isEmpty()) out << "," << note;
//        out << table->item(row, 5)->text();
//        out << "\n";
//    */

    return;
}

void MainWindow::bRecording(){
    view->recording = recordCheckBox->isChecked();
}
