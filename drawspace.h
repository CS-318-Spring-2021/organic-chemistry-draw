#ifndef DRAWSPACE_H
#define DRAWSPACE_H

#include <QWidget>
#include <QPoint>
#include <QColor>


class drawspace : public QWidget
{
    Q_OBJECT

private:
    void newPage();
    void closeEvent(QCloseEvent *event) override;
    void exportPage(QString fileType);


public:
    drawspace();

};

#endif // DRAWSPACE_H
