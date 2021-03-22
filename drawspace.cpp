#include "drawspace.h"
#include <QSurface>

#include <QtWidgets>

drawspace::drawspace() {
    setScene(&mScene);
    setSceneRect(QRectF(-400.0, -300.0, 800.0, 600.0));
    setRenderHint(QPainter::Antialiasing);

    QPainterPath crossPath;
    crossPath.moveTo(-4.0, 0.0); crossPath.lineTo(-1.0, 0.0);
    crossPath.moveTo(1.0, 0.0); crossPath.lineTo(4.0, 0.0);
    crossPath.moveTo(0.0, -4.0); crossPath.lineTo(0.0, -1.0);
    crossPath.moveTo(0.0, 1.0); crossPath.lineTo(0.0, 4.0);
    crosshairs = mScene.addPath(crossPath, QPen(Qt::red, 1.0));
    crosshairs->setVisible(false);
}

void PollingGraphicsView::setCrosshairsVisible(bool visible) {
    crosshairs->setVisible(visible);
}

void PollingGraphicsView::setCrosshairsPos(const QPointF &pos) {
    crosshairs->setPos(pos);
}

void PollingGraphicsView::mousePressEvent(QMouseEvent *evt) {
    QPointF pos = mapToScene(evt->pos());
    emit mouseEvent(MousePressed, QDateTime::currentMSecsSinceEpoch(), pos);
    lastPos = pos;
}

void PollingGraphicsView::maybeAddSegment(const QPointF &pos) {
    if (lastPos!=pos) {
        mScene.addLine(QLineF(lastPos, pos), QPen(Qt::black, 2.0));
        lastPos = pos;
    }
}

void PollingGraphicsView::mouseReleaseEvent(QMouseEvent *evt) {
    QPointF pos = mapToScene(evt->pos());
    emit mouseEvent(MouseReleased, QDateTime::currentMSecsSinceEpoch(), pos);
    maybeAddSegment(pos);
}

void PollingGraphicsView::mouseMoveEvent(QMouseEvent *evt) {
    QPointF pos = mapToScene(evt->pos());
    emit mouseEvent(MouseMoved, QDateTime::currentMSecsSinceEpoch(), pos);
    maybeAddSegment(pos);
}
