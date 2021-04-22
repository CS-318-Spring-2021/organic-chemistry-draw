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

void drawspace::setCrosshairsVisible(bool visible) {
    crosshairs->setVisible(visible);
}

void drawspace::setCrosshairsPos(const QPointF &pos) {
    crosshairs->setPos(pos);
}

void drawspace::mousePressEvent(QMouseEvent *evt) {
    QPointF pos = mapToScene(evt->pos());
    emit mouseEvent(MousePressed, QDateTime::currentMSecsSinceEpoch(), pos);
    lastPos = pos;
}

void drawspace::maybeAddSegment(const QPointF &pos) {
    if (lastPos!=pos) {
        mScene.addLine(QLineF(lastPos, pos), QPen(Qt::gray, 2.0));
        lastPos = pos;
    }
}

void drawspace::mouseReleaseEvent(QMouseEvent *evt) {
    QPointF pos = mapToScene(evt->pos());
    mScene.clear();
    emit mouseEvent(MouseReleased, QDateTime::currentMSecsSinceEpoch(), pos);
    //maybeAddSegment(pos);
}

void drawspace::mouseMoveEvent(QMouseEvent *evt) {
    QPointF pos = mapToScene(evt->pos());
    emit mouseEvent(MouseMoved, QDateTime::currentMSecsSinceEpoch(), pos);
    maybeAddSegment(pos);
}

void drawspace::replaceSegment(const QPointF &firstPos, const QPointF &lastPos) {
    mScene.addLine(QLineF(firstPos, lastPos), QPen(Qt::black, 2.0));
}

void drawspace::replaceSegment(const QPointF &firstPos, const QPointF &lastPos, QPen pen) {
    mScene.addLine(QLineF(firstPos, lastPos), pen);
}

void drawspace::replaceSegment(const QLineF line){
    mScene.addLine(line);
}

void drawspace::drawDimensionalBond(const QPointF &firstPos, const QPointF &lastPos, int style){
    //xy coordinates for the two endpoints of the bond
    const double TRIANGLE = 0.05;
    QLineF line(lastPos, firstPos);
    QLineF supplementary(lastPos, firstPos);
    supplementary.setLength(TRIANGLE*line.length());
    supplementary.setAngle(line.angle()+90);
    QPointF cornerone = supplementary.p2();
    supplementary.setAngle(line.angle()-90);
    QPointF cornertwo = supplementary.p2();
    mScene.addLine(QLineF(cornerone, cornertwo));

    QVector<QPointF> points;
    points.append(firstPos);
    points.append(cornerone);
    points.append(cornertwo);

    if (style==1){
        mScene.addPolygon(QPolygonF(points), QPen(Qt::black, 2.0), QBrush(Qt::SolidPattern));
    }
    if (style==2){
        QBrush br(Qt::VerPattern);
        QTransform t;
        t = t.inverted();
        t.rotate(0-line.angle()).scale(1,1);
        br.setTransform(t);
        mScene.addPolygon(QPolygonF(points), QPen(Qt::white, 0), br);
    }
}

void drawspace::drawMultipleBond(const QPointF &firstPos, const QPointF &lastPos, int num){
    double LENGTHMODIFIER = 0.018;
    if (num==2){
        LENGTHMODIFIER = 0.03;
    }
    QLineF line(firstPos, lastPos);
    QLineF supplementary(firstPos, lastPos);
    supplementary.setLength(LENGTHMODIFIER*line.length());
    supplementary.setAngle(line.angle()+90);
    QPointF cornerone = supplementary.p2();
    supplementary.setAngle(line.angle()-90);
    QPointF cornertwo = supplementary.p2();

    supplementary = QLineF(lastPos, firstPos);
    supplementary.setLength(LENGTHMODIFIER*line.length());
    supplementary.setAngle(line.angle()+90);
    QPointF cornerthree = supplementary.p2();
    supplementary.setAngle(line.angle()-90);
    QPointF cornerfour = supplementary.p2();

    if (num==2){
        mScene.addLine(line);
    }
    mScene.addLine(QLineF(cornerone, cornerthree));
    mScene.addLine(QLineF(cornertwo, cornerfour));
}
