#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <drawspace.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void maybeSave();
    void newPage();

    QAction *newPageAction();
    QAction *checkChangesAction();
    QAction *SaveAction();
    QAction *saveFileAction();

    drawspace *drawspace();

    QMenu *saveMenu;
    QMenu *openMenu;
    QMenu *optionsMenu;
    QMenu *filesMenu;


private slots:
    void deletePage();
    void exportPage(QString fileType);
    void penColor();
    void penWidth();
};
#endif // MAINWINDOW_H
