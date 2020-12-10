#include "picturetoolbar.h"
#include "pub_pushbutton.h"
#include "uiglobal.h"

#include <QHBoxLayout>
#include <QPainter>

class PictureToolBar::PrivateData
{
public:
    explicit PrivateData(PictureToolBar *parent) :
        q(parent), isAdaptive1x1_(true)
    {}

    PictureToolBar* q;

    uint isAdaptive1x1_ : 1;

    PubPushButton *openFileButton_;         /*<打开图片 */
    PubPushButton *editButton_;             /*<编辑图片 */
    PubPushButton *adaptiveButton_;         /*<在图片适应窗口和原图显示之间切换 */
    PubPushButton *zoomInButton_;           /*<放大图片 */
    PubPushButton *zoomOutButton_;          /*<缩小图片 */
    PubPushButton *toPreviousButton_;       /*<浏览上一张图片 */
    PubPushButton *toNextButton_;           /*<浏览下一张图片 */
    PubPushButton *rotateLeftButton_;       /*<向左旋转图片 */
    PubPushButton *rotateRightButton_;      /*<向右旋转图片 */

    void init();
    void updateAdaptiveButtonState();
};

PictureToolBar::PictureToolBar(QWidget *parent) : QWidget(parent),
    d(new PrivateData(this))
{
    d->init();
}

PictureToolBar::~PictureToolBar()
{
    delete d;
}

void PictureToolBar::setAdaptiveType(int type)
{
    d->isAdaptive1x1_ = (type == 0x02);
    d->updateAdaptiveButtonState();
}

void PictureToolBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), G_UISETTIGNS->themeColor());
}

void PictureToolBar::onButtonClicked()
{
    PubPushButton *button = qobject_cast<PubPushButton*>(sender());

    if (d->openFileButton_ == button) {
        emit clicked(BT_OpenFile);
    }
    else if (d->editButton_ == button) {
        emit clicked(BT_Edit);
    }
    else if (d->adaptiveButton_ == button) {
        emit clicked(BT_Adaptive);
    }
    else if (d->zoomInButton_ == button) {
        emit clicked(BT_ZoomIn);
    }
    else if (d->zoomOutButton_ == button) {
        emit clicked(BT_ZoomOut);
    }
    else if (d->toPreviousButton_ == button) {
        emit clicked(BT_ToPrevious);
    }
    else if (d->toNextButton_ == button) {
        emit clicked(BT_ToNext);
    }
    else if (d->rotateLeftButton_ == button) {
        emit clicked(BT_RotateLeft);
    }
    else if (d->rotateRightButton_ == button) {
        emit clicked(BT_RotateRight);
    }
}

void PictureToolBar::onThemeChanged(int type)
{
    Q_UNUSED(type)
    d->openFileButton_->setBackground(RCC_WRAPPER("hy_ps_openfile_01.png"));
    d->editButton_->setBackground(RCC_WRAPPER("hy_ps_edit_01.png"));
    d->updateAdaptiveButtonState();
    d->zoomInButton_->setBackground(RCC_WRAPPER("hy_ps_zoomin_01.png"));
    d->zoomOutButton_->setBackground(RCC_WRAPPER("hy_ps_zoomout_01.png"));
    d->toPreviousButton_->setBackground(RCC_WRAPPER("hy_ps_toprevious_01.png"));
    d->toNextButton_->setBackground(RCC_WRAPPER("hy_ps_tonext_01.png"));
    d->rotateLeftButton_->setBackground(RCC_WRAPPER("hy_ps_rotateleft_01.png"));
    d->rotateRightButton_->setBackground(RCC_WRAPPER("hy_ps_rotateright_01.png"));
    update();
}

void PictureToolBar::PrivateData::init()
{
    int barHeight = 46;
    int iconHeight = barHeight - 4;
    QSize iconSize(iconHeight, iconHeight);

    q->setFixedHeight(barHeight);
    q->setMouseTracking(true);

    openFileButton_ = new PubPushButton(q);
    openFileButton_->setFixedSize(iconSize);
    openFileButton_->setMouseTracking(true);
    QObject::connect(openFileButton_, &PubPushButton::clicked, [&]{
        emit q->clicked(PictureToolBar::BT_OpenFile);
    });

    editButton_ = new PubPushButton(q);
    editButton_->setFixedSize(iconSize);
    editButton_->setMouseTracking(true);
    QObject::connect(editButton_, &PubPushButton::clicked, [&]{
        emit q->clicked(PictureToolBar::BT_Edit);
    });

    adaptiveButton_ = new PubPushButton(q);
    adaptiveButton_->setFixedSize(iconSize);
    adaptiveButton_->setMouseTracking(true);
    QObject::connect(adaptiveButton_, &PubPushButton::clicked, [&]{
        emit q->clicked(PictureToolBar::BT_Adaptive);
    });

    zoomInButton_ = new PubPushButton(q);
    zoomInButton_->setFixedSize(iconSize);
    zoomInButton_->setMouseTracking(true);
    QObject::connect(zoomInButton_, &PubPushButton::clicked, [&]{
        emit q->clicked(PictureToolBar::BT_ZoomIn);
    });

    zoomOutButton_ = new PubPushButton(q);
    zoomOutButton_->setFixedSize(iconSize);
    zoomOutButton_->setMouseTracking(true);
    QObject::connect(zoomOutButton_, &PubPushButton::clicked, [&]{
        emit q->clicked(PictureToolBar::BT_ZoomOut);
    });

    toPreviousButton_ = new PubPushButton(q);
    toPreviousButton_->setFixedSize(iconSize);
    toPreviousButton_->setMouseTracking(true);
    QObject::connect(toPreviousButton_, &PubPushButton::clicked, [&]{
        emit q->clicked(PictureToolBar::BT_ToPrevious);
    });

    toNextButton_ = new PubPushButton(q);
    toNextButton_->setFixedSize(iconSize);
    toNextButton_->setMouseTracking(true);
    QObject::connect(toNextButton_, &PubPushButton::clicked, [&]{
        emit q->clicked(PictureToolBar::BT_ToNext);
    });

    rotateLeftButton_ = new PubPushButton(q);
    rotateLeftButton_->setFixedSize(iconSize);
    rotateLeftButton_->setMouseTracking(true);
    QObject::connect(rotateLeftButton_, &PubPushButton::clicked, [&]{
        emit q->clicked(PictureToolBar::BT_RotateLeft);
    });

    rotateRightButton_ = new PubPushButton(q);
    rotateRightButton_->setFixedSize(iconSize);
    rotateRightButton_->setMouseTracking(true);
    QObject::connect(rotateRightButton_, &PubPushButton::clicked, [&]{
        emit q->clicked(PictureToolBar::BT_RotateRight);
    });

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 4);
    layout->setSpacing(10);
    layout->addStretch();
    layout->addWidget(openFileButton_);
    layout->addWidget(editButton_);
    layout->addWidget(adaptiveButton_);
    layout->addWidget(zoomInButton_);
    layout->addWidget(zoomOutButton_);
    layout->addWidget(toPreviousButton_);
    layout->addWidget(toNextButton_);
    layout->addWidget(rotateLeftButton_);
    layout->addWidget(rotateRightButton_);
    layout->addStretch();
    q->setLayout(layout);

    QObject::connect(G_UISETTIGNS, &UiGlobalSettings::themeChanged, q, &PictureToolBar::onThemeChanged);
}

void PictureToolBar::PrivateData::updateAdaptiveButtonState()
{
    if (isAdaptive1x1_) {
        adaptiveButton_->setBackground(RCC_WRAPPER("hy_ps_1x1_01.png"));
    } else {
        adaptiveButton_->setBackground(RCC_WRAPPER("hy_ps_adaptive_01.png"));
    }
}
