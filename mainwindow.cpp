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
    rightLayout->addWidget(saveButton = new QPushButton("Save"));


    connect(saveButton, &QPushButton::clicked, this, &MainWindow::bSave);

    QVBoxLayout *taskBarLayout = new QVBoxLayout();
    taskBarLayout->addWidget(recordCheckBox = new QCheckBox("Record"));
    taskBarLayout->addWidget(undoCheckBox = new QCheckBox("Undo"));

    recordCheckBox->setCheckable(true);
    recordCheckBox->setChecked(true);

    connect(recordCheckBox, &QCheckBox::stateChanged, this, &MainWindow::bRecording);

    undoCheckBox->setCheckable(true);
    undoCheckBox->setChecked(false);

    rightLayout->addWidget(undoButton = new QPushButton("Undo"));


    connect(undoButton, &QPushButton::clicked, this, &MainWindow::bUndo);


    mainLayout->addLayout(taskBarLayout);
    mainLayout->addWidget(view = new drawspace(), 1);
    mainLayout->addLayout(rightLayout);
    bRecording();


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
    /*
    for (int row = 0; row < table->rowCount(); ++row) {
        out << table->item(row, 0)->text() << "," << table->item(row, 1)->text() << "," << table->item(row, 2)->text() << "," << table->item(row, 3)->text();
        QString note = table->item(row, 4)->text();
        if (!note.isEmpty()) out << "," << note;
        out << table->item(row, 5)->text();
        out << "\n";
    */

    return;
}

void MainWindow::bRecording(){
    view->recording = recordCheckBox->isChecked();
}

void MainWindow::bUndo(){
    if(view->undoStack.size() <= 1) {
        view->undoStack.clear();
        view->molecules.clear();
        view->mScene.clear();
        return;
    }
    view->undoStack.pop_back();
    view->molecules = view->undoStack.last();
    view->mScene.clear();
    if(view->undoStack.size() > 1) view->drawExisting();

}
