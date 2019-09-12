#include "pub_subwindow.h"

#include <QMouseEvent>

PubSubWindow::PubSubWindow(QWidget *parent) : QWidget(parent),
    isPress_(false)
{
    setWindowFlags(Qt::FramelessWindowHint);
}

void PubSubWindow::mousePressEvent(QMouseEvent *e)
{
    isPress_ = true;
    lastGlobalPos_ = e->globalPos();
    QWidget::mousePressEvent(e);
}

void PubSubWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (isPress_) {
        move(this->pos() + e->globalPos() - lastGlobalPos_);
        lastGlobalPos_ = e->globalPos();
    }
    QWidget::mouseMoveEvent(e);
}

void PubSubWindow::mouseReleaseEvent(QMouseEvent *e)
{
    isPress_ = false;
    QWidget::mouseReleaseEvent(e);
}

