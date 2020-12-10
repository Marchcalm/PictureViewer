#include "maintitlebar.h"

#include "pub_label.h"
#include "pub_pushbutton.h"
#include "uiglobal.h"

#include <QPainter>
#include <QHBoxLayout>
#include <QMenu>

namespace TR {
static const QString CoolBlack(QObject::tr("Cool Black"));
static const QString ClassicWhite(QObject::tr("Classic White"));
static const QString Settings(QObject::tr("Settings"));
}

class MainTitleBar::PrivateData
{
public:
    explicit PrivateData(MainTitleBar *parent)
        : q(parent), windwowState_(Qt::WindowNoState) {}

    MainTitleBar *q;
    Qt::WindowStates windwowState_;

    PubLabel *logoLabel_;
    PubLabel *titleLabel_;
    PubPushButton *skinButton_;
    PubPushButton *PrimaryMenuButton_;
    PubPushButton *fullScreenButton_;
    PubPushButton *minimizeButton_;
    PubPushButton *maximizeButton_;
    PubPushButton *closeButton_;

    void init();
    void popupSkinMenu();
    void popupPrimaryMenu();
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

void MainTitleBar::onWindowStateChanged(Qt::WindowStates state)
{
    d->windwowState_ = state;
    switch (state) {
    case Qt::WindowNoState:
        d->maximizeButton_->setBackground(RCC_WRAPPER("hy_main_maximize_01.png"));
        d->fullScreenButton_->setBackground(RCC_WRAPPER("hy_main_fullscreen_01.png"));
        break;
    case Qt::WindowMaximized:
        d->maximizeButton_->setBackground(RCC_WRAPPER("hy_main_maximize_02.png"));
        break;
    case Qt::WindowFullScreen:
        d->fullScreenButton_->setBackground(RCC_WRAPPER("hy_main_fullscreen_02.png"));
        break;
    default:
        break;
    }
}

void MainTitleBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), G_UISETTIGNS->themeColor());
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
    else if (d->PrimaryMenuButton_ == button) {
        d->popupPrimaryMenu();
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

void MainTitleBar::onThemeChanged(int type)
{
    switch (type) {
    case UiGlobalSettings::TT_CoolBlack:
        d->titleLabel_->setTextColor(Qt::white);
        break;
    case UiGlobalSettings::TT_ClassicWhite:
        d->titleLabel_->setTextColor(QColor(40, 40, 40));
        break;
    default:
        break;
    }

    onWindowStateChanged(d->windwowState_);

    d->logoLabel_->setPixmap(RCC_WRAPPER("hy_main_logo_01.png"));
    d->skinButton_->setBackground(RCC_WRAPPER("hy_main_skin_01.png"));
    d->PrimaryMenuButton_->setBackground(RCC_WRAPPER("hy_main_menu_01.png"));
    d->minimizeButton_->setBackground(RCC_WRAPPER("hy_main_minimize_01.png"));
    d->closeButton_->setBackground(RCC_WRAPPER("hy_main_close_01.png"));
    update();
}

void MainTitleBar::PrivateData::init()
{
    int barHeight = 30;
    int iconHeight = barHeight-2;
    QSize iconSize(iconHeight, iconHeight);

    q->setFixedHeight(barHeight);
    q->setMouseTracking(true);

    logoLabel_ = new PubLabel(q);   
    logoLabel_->setFixedSize(iconSize);
    logoLabel_->setMouseTracking(true);

    titleLabel_ = new PubLabel(q);
    titleLabel_->setText(G_UISETTIGNS->appName(), false);
    titleLabel_->setMinimumSize(300, iconHeight);
    titleLabel_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    titleLabel_->setPointSize(10);
    titleLabel_->setMouseTracking(true);

    skinButton_ = new PubPushButton(q);   
    skinButton_->setFixedSize(iconSize);
    skinButton_->setMouseTracking(true);
    QObject::connect(skinButton_, &PubPushButton::pressed, q, &MainTitleBar::onButtonClicked);

    PrimaryMenuButton_ = new PubPushButton(q);
    PrimaryMenuButton_->setFixedSize(iconSize);
    PrimaryMenuButton_->setMouseTracking(true);
    QObject::connect(PrimaryMenuButton_, &PubPushButton::clicked, q, &MainTitleBar::onButtonClicked);

    fullScreenButton_ = new PubPushButton(q);
    fullScreenButton_->setFixedSize(iconSize);
    fullScreenButton_->setMouseTracking(true);
    QObject::connect(fullScreenButton_, &PubPushButton::clicked, q, &MainTitleBar::onButtonClicked);

    minimizeButton_ = new PubPushButton(q);
    minimizeButton_->setFixedSize(iconSize);
    minimizeButton_->setMouseTracking(true);
    QObject::connect(minimizeButton_, &PubPushButton::clicked, q, &MainTitleBar::onButtonClicked);

    maximizeButton_ = new PubPushButton(q);
    maximizeButton_->setFixedSize(iconSize);
    maximizeButton_->setMouseTracking(true);
    QObject::connect(maximizeButton_, &PubPushButton::clicked, q, &MainTitleBar::onButtonClicked);

    closeButton_ = new PubPushButton(q);   
    closeButton_->setFixedSize(iconSize);
    closeButton_->setBackgroundColorByHovered(QColor(205, 0, 0, 150));
    closeButton_->setBackgroundColorByPressed(QColor(205, 0, 0, 255));
    closeButton_->setMouseTracking(true);
    QObject::connect(closeButton_, &PubPushButton::clicked, q, &MainTitleBar::onButtonClicked);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(4, 0, 2, 4);
    layout->setSpacing(2);
    layout->addWidget(logoLabel_);
    layout->addWidget(titleLabel_);
    layout->addWidget(skinButton_);
    layout->addWidget(PrimaryMenuButton_);
    layout->addWidget(fullScreenButton_);
    layout->addWidget(minimizeButton_);
    layout->addWidget(maximizeButton_);
    layout->addWidget(closeButton_);
    q->setLayout(layout);

    QObject::connect(G_UISETTIGNS, &UiGlobalSettings::themeChanged, q, &MainTitleBar::onThemeChanged);
}

void MainTitleBar::PrivateData::popupSkinMenu()
{
    QMenu *menu = new QMenu;
    menu->setStyleSheet(QLatin1String("QMenu {"
                                      "background-color: white;"
                                      "border: 1px solid #D7D7D7;"
                                      "}"
                                      "QMenu::item {"
                                      "height: 30px;"
                                      "padding: 0px 30px 0px 20px;"
                                      "}"
                                      "QMenu::item:selected {"
                                      "background: rgba(120, 120, 120, 120);"
                                      "}"
                                      "QMenu::icon {"
                                      "margin-left: 12px;"
                                      "}"
                                      ));
    menu->setAttribute(Qt::WA_DeleteOnClose);    
    menu->addAction(TR::CoolBlack);
    menu->addAction(TR::ClassicWhite);

    menu->actions().at(G_UISETTIGNS->themeType())->setIcon(QIcon(RCC_WRAPPER("hy_main_gou_01.png")));

    QObject::connect(menu, &QMenu::triggered, [&](QAction* action){
        if (TR::CoolBlack == action->text()) {
            G_UISETTIGNS->setTheme(UiGlobalSettings::TT_CoolBlack);
        }
        else if (TR::ClassicWhite == action->text()) {
            G_UISETTIGNS->setTheme(UiGlobalSettings::TT_ClassicWhite);
        }
    });

    QPoint pos = q->mapToGlobal(skinButton_->pos());
    pos.setX(pos.x() + 15);
    pos.setY(pos.y() + 40);
    menu->popup(pos);
}

void MainTitleBar::PrivateData::popupPrimaryMenu()
{
    QMenu *menu = new QMenu;
    menu->setStyleSheet(QLatin1String("QMenu {"
                                      "background-color: white;"
                                      "border: 1px solid #D7D7D7;"
                                      "}"
                                      "QMenu::item {"
                                      "height: 30px;"
                                      "padding: 0px 30px 0px 20px;"
                                      "}"
                                      "QMenu::item:selected {"
                                      "background: rgba(120, 120, 120, 120);"
                                      "}"
                                      "QMenu::icon {"
                                      "margin-left: 12px;"
                                      "}"
                                      "QMenu::separator {"
                                      "height: 1px;"
                                      "background: #ccc;"
                                      "margin-top: 2px;"
                                      "margin-bottom: 2px;"
                                      "margin-left: 5px;"
                                      "margin-right: 5px;"
                                      "}"
                                      ));
    menu->setAttribute(Qt::WA_DeleteOnClose);

    menu->addAction(QString("About %1").arg(G_UISETTIGNS->appName()));
    menu->addSeparator();
    menu->addAction(new QAction(QIcon(RCC_WRAPPER("hy_main_settings_01.png")), TR::Settings));

    QObject::connect(menu, &QMenu::triggered, [&](QAction* action){
        if (TR::Settings == action->text()) {
            emit q->actionTriggered(MainTitleBar::AT_AppSettings);
        }
    });

    QPoint pos = q->mapToGlobal(PrimaryMenuButton_->pos());
    pos.setX(pos.x() + 15);
    pos.setY(pos.y() + 40);
    menu->popup(pos);
}
