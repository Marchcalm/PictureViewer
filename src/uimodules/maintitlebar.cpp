#include "maintitlebar.h"

#include "pub_label.h"
#include "pub_pushbutton.h"
#include "uiglobal.h"

#include <QPainter>
#include <QHBoxLayout>
#include <QMenu>

#include <QDebug>

namespace TR {
static const QString CoolBlack(QObject::tr("Cool Black"));
static const QString ClassicWhite(QObject::tr("Classic White"));
}

class MainTitleBar::PrivateData
{
public:
    explicit PrivateData(MainTitleBar *parent)
        : q(parent) {}

    MainTitleBar *q;

    PubLabel *logoLabel_;
    PubLabel *titleLabel_;

    PubPushButton *skinButton_;
    PubPushButton *menuButton_;
    PubPushButton *fullScreenButton_;
    PubPushButton *minimizeButton_;
    PubPushButton *maximizeButton_;
    PubPushButton *closeButton_;

    void init();
    void popupSkinMenu();
};

MainTitleBar::MainTitleBar(QWidget *parent) : QWidget(parent),
    d(new PrivateData(this))
{
    d->init();
}

MainTitleBar::~MainTitleBar()
{
    delete d;
}

void MainTitleBar::setTitle(const QString &text)
{
    d->titleLabel_->setText(text);
}

void MainTitleBar::handleWindowState(Qt::WindowStates state)
{
    switch (state) {
    case Qt::WindowNoState:
        d->maximizeButton_->setBackground(QLatin1String("E:/workspace/qt/PictureViewer/skin/maximize2.png"));
        break;
    case Qt::WindowMinimized:
        break;
    case Qt::WindowMaximized:
        d->maximizeButton_->setBackground(QLatin1String("E:/workspace/qt/PictureViewer/skin/maximize_01.png"));
        break;
    case Qt::WindowFullScreen:
        break;
    default:
        break;
    }
}

void MainTitleBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), UiGlobalSettings::obj()->thmeColor());
}

void MainTitleBar::mouseDoubleClickEvent(QMouseEvent *e)
{
    emit clicked(MainTitleBar::BT_Maximize);
    QWidget::mouseDoubleClickEvent(e);
}

void MainTitleBar::onButtonClicked()
{
    PubPushButton *button = qobject_cast<PubPushButton*>(sender());

    if (d->skinButton_ == button) {
        d->popupSkinMenu();
    }
    else if (d->menuButton_ == button) {

    }
    else if (d->fullScreenButton_ == button) {
        emit clicked(MainTitleBar::BT_FullScreen);
    }
    else if (d->minimizeButton_ == button) {
        emit clicked(MainTitleBar::BT_Minimize);
    }
    else if (d->maximizeButton_ == button) {
        emit clicked(MainTitleBar::BT_Maximize);
    }
    else if (d->closeButton_ == button) {
        emit clicked(MainTitleBar::BT_Close);
    }
}

void MainTitleBar::PrivateData::init()
{
    int barHeight = 28;
    int iconHeight = barHeight;
    QSize iconSize(iconHeight, iconHeight);

    q->setFixedHeight(barHeight);
    q->setMouseTracking(true);

    logoLabel_ = new PubLabel(q);
    logoLabel_->setPixmap(QLatin1String(":/images/picture2.png"));
    logoLabel_->setFixedSize(iconSize);
    logoLabel_->setMouseTracking(true);

    titleLabel_ = new PubLabel(q);
    titleLabel_->setMinimumSize(300, iconHeight);
    titleLabel_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    titleLabel_->setTextColor(Qt::white);
    QFont font(titleLabel_->font());
    font.setPixelSize(14);
    titleLabel_->setFont(font);
    titleLabel_->setMouseTracking(true);

    skinButton_ = new PubPushButton(q);
    skinButton_->setBackground(QLatin1String(":/CoolBlack/skin_02.png"));
    skinButton_->setFixedSize(iconSize);
    skinButton_->setMouseTracking(true);
    QObject::connect(skinButton_, &PubPushButton::pressed, q, &MainTitleBar::onButtonClicked);

    menuButton_ = new PubPushButton(q);
    menuButton_->setBackground(QLatin1String("E:/workspace/qt/PictureViewer/skin/menu.png"));
    menuButton_->setFixedSize(iconSize);
    menuButton_->setMouseTracking(true);
    QObject::connect(menuButton_, &PubPushButton::clicked, q, &MainTitleBar::onButtonClicked);

    fullScreenButton_ = new PubPushButton(q);
    fullScreenButton_->setBackground(QLatin1String("E:/workspace/qt/PictureViewer/skin/fullScreen.png"));
    fullScreenButton_->setFixedSize(iconSize);
    fullScreenButton_->setMouseTracking(true);
    QObject::connect(fullScreenButton_, &PubPushButton::clicked, q, &MainTitleBar::onButtonClicked);

    minimizeButton_ = new PubPushButton(q);
    minimizeButton_->setBackground(QLatin1String("E:/workspace/qt/PictureViewer/skin/minimize1.png"));
    minimizeButton_->setFixedSize(iconSize);
    minimizeButton_->setMouseTracking(true);
    QObject::connect(minimizeButton_, &PubPushButton::clicked, q, &MainTitleBar::onButtonClicked);

    maximizeButton_ = new PubPushButton(q);
    maximizeButton_->setBackground(QLatin1String("E:/workspace/qt/PictureViewer/skin/maximize4.png"));
    maximizeButton_->setFixedSize(iconSize);
    maximizeButton_->setMouseTracking(true);
    QObject::connect(maximizeButton_, &PubPushButton::clicked, q, &MainTitleBar::onButtonClicked);

    closeButton_ = new PubPushButton(q);
    closeButton_->setBackground(QLatin1String(":/images/hy_close_01.png"));
    closeButton_->setFixedSize(iconSize);
    closeButton_->setMouseTracking(true);
    QObject::connect(closeButton_, &PubPushButton::clicked, q, &MainTitleBar::onButtonClicked);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    layout->addWidget(logoLabel_);
    layout->addWidget(titleLabel_);
    layout->addStretch();
    layout->addWidget(skinButton_);
    layout->addWidget(menuButton_);
    layout->addWidget(fullScreenButton_);
    layout->addWidget(minimizeButton_);
    layout->addWidget(maximizeButton_);
    layout->addWidget(closeButton_);
    q->setLayout(layout);

    QObject::connect(UiGlobalSettings::obj(), &UiGlobalSettings::themeChanged, q,
                     static_cast<void (MainTitleBar::*)()>(&MainTitleBar::update));
}

void MainTitleBar::PrivateData::popupSkinMenu()
{
    QMenu *menu = new QMenu;
    menu->setAttribute(Qt::WA_DeleteOnClose);
    menu->addAction(TR::CoolBlack);
    menu->addAction(TR::ClassicWhite);

    QObject::connect(menu, &QMenu::triggered, [&](QAction* action){
        if (TR::CoolBlack == action->text()) {
            UiGlobalSettings::obj()->setTheme(UiGlobalSettings::TT_CoolBlack);
        }
        else if (TR::ClassicWhite == action->text()) {
            UiGlobalSettings::obj()->setTheme(UiGlobalSettings::TT_ClassicWhite);
        }
    });

    QPoint pos = q->mapToGlobal(skinButton_->pos());
    pos.setX(pos.x() + 15);
    pos.setY(pos.y() + 40);
    menu->popup(pos);
}
