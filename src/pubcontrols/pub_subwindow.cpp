#include "pub_subwindow.h"

#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>

static const int kPadding = 20;

PubSubWindow::PubSubWindow(QWidget *parent) : QWidget(parent),
    isPress_(false)
{
    setWindowFlags(Qt::FramelessWindowHint);

    setAttribute(Qt::WA_TranslucentBackground);

    contentWidget_ = new QWidget(this);
    contentWidget_->setMouseTracking(true);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(kPadding >> 1);
    mainLayout->addWidget(contentWidget_);
    setBackgroundColor(QColor(255, 255, 255));
    setLayout(mainLayout);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(QColor(30, 30, 30));
    shadowEffect->setBlurRadius(kPadding >> 1);
    contentWidget_->setGraphicsEffect(shadowEffect);
}

QWidget *PubSubWindow::contentWidget() const
{
    return contentWidget_;
}

void PubSubWindow::setBackgroundColor(const QColor &color)
{
    if (!contentWidget_) return;

    QPalette pal(contentWidget_->palette());
    pal.setColor(QPalette::Window, color);
    contentWidget_->setPalette(pal);
    contentWidget_->setAutoFillBackground(true);
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

