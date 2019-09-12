#include "picturequickviewbutton.h"

#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class PictureQuickViewButton::PrivateData
{
public:
    explicit PrivateData(PictureQuickViewButton *parent) :
        q(parent), opacityEffect_(nullptr), animation_(nullptr),
        isReadyShow_(true)
    {}

    PictureQuickViewButton *q;

    QGraphicsOpacityEffect *opacityEffect_;
    QPropertyAnimation *animation_;
    uint isReadyShow_ : 1;

    void init();
};

PictureQuickViewButton::PictureQuickViewButton(QWidget *parent) : PubPushButton(parent),
    d(new PrivateData(this))
{
    d->init();
}

PictureQuickViewButton::~PictureQuickViewButton()
{
    delete d;
}

void PictureQuickViewButton::requestShow()
{
    d->isReadyShow_ = true;

    if (d->animation_->state() == QPropertyAnimation::Running) {
        d->animation_->stop();
    }

    QGraphicsOpacityEffect *opacityEffect = static_cast<QGraphicsOpacityEffect*>(graphicsEffect());
    if (opacityEffect)  {
        delete opacityEffect;
        setGraphicsEffect(nullptr);
    }

    if (isHidden())
        show();
}

void PictureQuickViewButton::requestHide()
{
    if (d->animation_->state() == QPropertyAnimation::Running)
        return;

    if (!graphicsEffect()) {
        QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
        opacityEffect->setOpacity(1.0);
        setGraphicsEffect(opacityEffect);

        d->animation_ ->setTargetObject(opacityEffect);
    }

    d->isReadyShow_ = false;
    d->animation_->start();
}

void PictureQuickViewButton::PrivateData::init()
{
    animation_ = new QPropertyAnimation(q);
    animation_->setPropertyName("opacity");
    animation_->setDuration(1500);
    animation_->setStartValue(1.0);
    animation_->setEndValue(0.0);
    connect(animation_, &QPropertyAnimation::finished, [&]() {
        if (!isReadyShow_) {
            q->hide();
        }
    });
}
