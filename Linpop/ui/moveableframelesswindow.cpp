
#include "moveableframelesswindow.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QRect>
#include <QPoint>
#include <QGridLayout>

MoveableFramelessWindow::MoveableFramelessWindow(QWidget* ptParent) :
    QWidget(ptParent), m_bIsMove(false) {
    // 设置窗体为无边框
    this->setWindowFlags(Qt::FramelessWindowHint);

    // 设置底层背景透明
//    setAttribute(Qt::WA_TranslucentBackground);
}

void MoveableFramelessWindow::mousePressEvent(QMouseEvent* ptEvent) {
    // 判断位置
    bool bShouldMove = isPointInDragnWidget(getDragnWidget(), ptEvent->pos());

    if (bShouldMove) {
          m_tPressedPoint = ptEvent->pos();
          m_bIsMove = true;
    }

    ptEvent->ignore();

}

void MoveableFramelessWindow::mouseMoveEvent(QMouseEvent* ptEvent) {
    if ((Qt::LeftButton == ptEvent->buttons()) && m_bIsMove) {
            QPoint currPoint = this->pos();
            currPoint.setX(currPoint.x() + ptEvent->x() - m_tPressedPoint.x());
            currPoint.setY(currPoint.y() + ptEvent->y() - m_tPressedPoint.y());
            this->move(currPoint);
    }
}

void MoveableFramelessWindow::mouseReleaseEvent(QMouseEvent* ptEvent) {
    Q_UNUSED(ptEvent);
    m_bIsMove = false;
}

bool MoveableFramelessWindow::isPointInDragnWidget(const QWidget* ptWidget, const QPoint& rtPoint) {
    // 判断位置
    QRect tRect = ptWidget->rect();

    bool bIsIn = rtPoint.x() >= tRect.left() && rtPoint.x() <= tRect.right() &&
                 rtPoint.y() >= tRect.top()  && rtPoint.y() <= tRect.bottom();

    return bIsIn;
}
