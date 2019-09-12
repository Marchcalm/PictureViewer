#include "pub_pushbutton.h"

#include <QPainter>
#include <QEvent>

PubPushButton::PubPushButton(QWidget *parent) : QAbstractButton(parent),
    isEnter_(false), adaptiveType_(AT_NoAdaptive), backgroundColorByHovered_(180, 180, 180, 60),
    backgroundColorByPressed_(30, 30, 30, 180)
{
}

PubPushButton::~PubPushButton()
{
}

void PubPushButton::setBackground(const QString &normal, const QString &press, const QString &check)
{
    QImage iamge;

    if (!normal.isEmpty()) {
        iamge.load(normal);
        backgroundPixmaps_[0] = QPixmap::fromImage(iamge);
    }

    if (!press.isEmpty()) {
        iamge.load(press);
        backgroundPixmaps_[1] = QPixmap::fromImage(iamge);
    }

    if (!check.isEmpty()) {
        iamge.load(check);
        backgroundPixmaps_[2] = QPixmap::fromImage(iamge);
        setCheckable(true);
    }

    update();
}

void PubPushButton::setAdaptiveType(PubPushButton::AadptiveType type)
{
    if (adaptiveType_ != type) {
        adaptiveType_ = type;
        update();
    }
}

QColor PubPushButton::backgroundColorByHovered() const
{
    return backgroundColorByHovered_;
}

void PubPushButton::setBackgroundColorByHovered(const QColor &c)
{
    backgroundColorByHovered_ = c;
}

QColor PubPushButton::backgroundColorByPressed() const
{
    return backgroundColorByPressed_;
}

void PubPushButton::setBackgroundColorByPressed(const QColor &c)
{
    backgroundColorByPressed_ = c;
}

void PubPushButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (isChecked()) {
        drawBackground(2, &painter);
    } else {
        if (isDown()) {
            drawBackground(1, &painter);
        } else {
            drawBackground(0, &painter);
        }
    }
}

bool PubPushButton::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::FocusOut:
        if (isDown()) {
            setDown(false);
            update();
        }
        break;
    case QEvent::Enter:
        isEnter_ = true;
        update();
        break;
    case QEvent::Leave:
        isEnter_ = false;
        update();
        break;
    default:
        break;
    }

    return QWidget::event(e);
}

void PubPushButton::drawBackground(int pixmapIndex, QPainter *painter)
{
    if (pixmapIndex < 3 && !backgroundPixmaps_[pixmapIndex].isNull()) {

        if (pixmapIndex == 0 && isEnter_) {
            painter->fillRect(rect(), backgroundColorByHovered_);
        }

        switch (adaptiveType_) {
        case PubPushButton::AT_NoAdaptive:
            painter->drawPixmap((width() - backgroundPixmaps_[pixmapIndex].width()) >> 1,
                                (height() - backgroundPixmaps_[pixmapIndex].height()) >> 1,
                                backgroundPixmaps_[pixmapIndex]);
            break;
        case PubPushButton::AT_Button:
            painter->drawPixmap(this->rect(), backgroundPixmaps_[pixmapIndex]);
            break;
        case PubPushButton::AT_Image: {
            QSize pixsize;
            if (isChecked()) {
                pixsize = backgroundPixmaps_[2].isNull() ? backgroundPixmaps_[0].size() : backgroundPixmaps_[2].size();
            } else {
                if (isDown()) {
                    pixsize = backgroundPixmaps_[1].isNull() ? backgroundPixmaps_[0].size() : backgroundPixmaps_[1].size();
                } else {
                    pixsize = backgroundPixmaps_[0].size();
                }
            }

            if (this->size() != pixsize) {
                resize(pixsize);
            }
            painter->drawPixmap(0, 0, backgroundPixmaps_[pixmapIndex]);
        }
            break;
        }

    } else {
        if (pixmapIndex == 1) {
            painter->fillRect(rect(), backgroundColorByPressed_);
            drawBackground(0, painter);
        }
    }
}
