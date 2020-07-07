
#ifndef MOVEABLEFRAMELESSWINDOW_H
#define MOVEABLEFRAMELESSWINDOW_H

#include <QObject>
#include <QWidget>
#include <QPoint>

class MoveableFramelessWindow : public QWidget {
    Q_OBJECT
public:
    explicit MoveableFramelessWindow(QWidget* ptParent);

protected:
    void mousePressEvent(QMouseEvent* ptEvent);
    void mouseMoveEvent(QMouseEvent* ptEvent);
    void mouseReleaseEvent(QMouseEvent* ptEvent);
	
    virtual QWidget* getDragnWidget() = 0;

    bool isPointInDragnWidget(const QWidget* ptWidget, const QPoint& rtPoint);

    bool   m_bIsMove;
    QPoint m_tPressedPoint;
};

#endif // MOVEABLEFRAMELESSWINDOW_H
