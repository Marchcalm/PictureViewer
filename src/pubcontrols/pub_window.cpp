#include "pub_window.h"

#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QEvent>

static const int kPadding = 16;

static int helperCalcPosition(int pos, int range)
{
    return (pos <= kPadding) ? 0 : ((pos > (range - kPadding)) ? 2 : 1);
}

PubWindow::PubWindow(QWidget *parent)
    : QWidget(parent),
      isPress_(false),
      cursorPosition_(11),
      cursorShepe_(Qt::ArrowCursor),
      contentWidget_(nullptr)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setMouseTracking(true);
    setMinimumSize(QSize(50, 50));

    contentWidget_ = new QWidget(this);
    contentWidget_->setMouseTracking(true);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(kPadding >> 1);
    mainLayout->addWidget(contentWidget_);
    setLayout(mainLayout);

    setShadowEffect(kPadding >> 1);
}

QWidget *PubWindow::contentWidget() const
{
    return contentWidget_;
}

void PubWindow::setBackgroundColor(const QColor &color)
{
    if (!contentWidget_) return;

    QPalette pal(contentWidget_->palette());
    pal.setColor(QPalette::Window, color);
    contentWidget_->setPalette(pal);
}

void PubWindow::mousePressEvent(QMouseEvent* e)
{
    isPress_ = true;
    lastGlobalPos_ = e->globalPos();
    cursorPosition_ = calcPosition(e->pos());
    QWidget::mousePressEvent(e);
}

void PubWindow::mouseReleaseEvent(QMouseEvent* e)
{
    isPress_ = false;
    QWidget::mouseReleaseEvent(e);
}

void PubWindow::mouseMoveEvent(QMouseEvent* e)
{
    if(isPress_) {
        QPoint curPos = e->globalPos();
        curPos -= lastGlobalPos_;

        if(11 == cursorPosition_) { //拖动
            switch (windowState()) {
            case Qt::WindowFullScreen:
                return;
            case Qt::WindowMaximized:
                if (lastGlobalPos_ != e->globalPos()) {
                    showNormal();
                    move(pos().x(), curPos.y());
                }
                break;
            default:
                move(curPos + pos());
                break;
            }
        } else { // 调整大小
            QRect r = geometry();
            switch(cursorPosition_) {
            case 00:
                r.setTopLeft(r.topLeft() + curPos);
                break;
            case 02:
                r.setTopRight(r.topRight() + curPos);
                break;
            case 20:
                r.setBottomLeft(r.bottomLeft() + curPos);
                break;
            case 22:
                r.setBottomRight(r.bottomRight() + curPos);
                break;
            case 10:
                r.setLeft(r.left() + curPos.x());
                break;
            case 12:
                r.setRight(r.right() + curPos.x());
                break;
            case 01:
                r.setTop(r.top() + curPos.y());
                break;
            case 21:
                r.setBottom(r.bottom() + curPos.y());
                break;
            default:
                break;
            }
            setGeometry(r);
        }
        lastGlobalPos_ = e->globalPos();
    } else {
        setCursorType(calcPosition(e->pos()));
    }

    QWidget::mouseMoveEvent(e);
}

void PubWindow::changeEvent(QEvent *e)
{
    if(e->type() != QEvent::WindowStateChange)
        return;

    Qt::WindowStates state = windowState();
    emit windowStateChaned(state);

    switch (state) {
    case Qt::WindowNoState: {
        if (layout()->margin() == 0) {
            layout()->setMargin(kPadding >> 1);
            setShadowEffect(kPadding >> 1);
        }
    }
        break;
    case Qt::WindowMaximized:
        layout()->setMargin(0);
        setShadowEffect(0.0);
        break;
    case Qt::WindowFullScreen:
        layout()->setMargin(0);
        setShadowEffect(0.0);
        break;
    default:
        break;
    }
}

int PubWindow::calcPosition(const QPoint& pt)
{
    if (windowState() == Qt::WindowMaximized ||
            windowState() == Qt::WindowFullScreen) {
        return 11;
    }

    int row = helperCalcPosition(pt.y(), rect().height());
    int col = helperCalcPosition(pt.x(), rect().width());
    return row * 10 + col;
}

void PubWindow::setCursorType(int value)
{
    Qt::CursorShape cursor;

    switch(value)
    {
    case 00:
    case 22:
        cursor = Qt::SizeFDiagCursor;
        break;
    case 02:
    case 20:
        cursor = Qt::SizeBDiagCursor;
        break;
    case 10:
    case 12:
        cursor = Qt::SizeHorCursor;
        break;
    case 01:
    case 21:
        cursor = Qt::SizeVerCursor;
        break;
    case 11:
        cursor = Qt::ArrowCursor;
        break;
    default:
        cursor = Qt::ArrowCursor;
        break;
    }

    if(cursor != cursorShepe_) {
        cursorShepe_ = cursor;
        setCursor(cursorShepe_);
    }
}

void PubWindow::setShadowEffect(qreal blurRadius)
{
    QGraphicsEffect *oldShadow = contentWidget_->graphicsEffect();
    if (oldShadow) {
        delete oldShadow;
        contentWidget_->setGraphicsEffect(nullptr);
    }

    if (blurRadius > 0.0) {
        QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;
        shadowEffect->setOffset(0, 0);
//        shadowEffect->setColor(QColor(60, 60, 60));
        shadowEffect->setColor(QColor(160, 0, 0));
        shadowEffect->setBlurRadius(blurRadius);
        contentWidget_->setGraphicsEffect(shadowEffect);
    }
}
