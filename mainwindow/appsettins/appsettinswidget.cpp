#include "appsettinswidget.h"
#include "pub_pushbutton.h"
#include "pub_label.h"
#include "appsettingstabbar.h"
#include "appsettingsbodywidget.h"
#include "uiglobal.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>

namespace TR {
static const QString SettingsCenter(QObject::tr("Settings Center"));
}

class AppSettinsWidget::PrivateData
{
public:
    explicit PrivateData(AppSettinsWidget *parent) :
        q(parent)
    {}

    AppSettinsWidget *q;

    AppSettingsTabBar *bodyTabBar_;
    AppSettingsBodyWidget *bodyContentWidget_;

    void init();
};

AppSettinsWidget::AppSettinsWidget(QWidget *parent) : PubSubWindow(parent),
    d(new PrivateData(this))
{
    d->init();
}

AppSettinsWidget::~AppSettinsWidget()
{
    delete d;
}

void AppSettinsWidget::PrivateData::init()
{
    q->setFixedSize(600, 410);
    q->setWindowFlags(q->windowFlags() | Qt::SubWindow);
    q->setAttribute(Qt::WA_DeleteOnClose);

    //! [0] titleBar
    int th = 30;
    int iconw = th - 2;
    QWidget *titleBar = new QWidget(q);
    titleBar->setFixedHeight(th);
    G_UISETTIGNS->setWidgetBackgroundColor(titleBar, QColor(140, 140, 140, 150));

    PubLabel *logoLabel = new PubLabel(titleBar);
    logoLabel->setFixedSize(iconw, iconw);
    logoLabel->setPixmap(RCC_WRAPPER("hy_main_settings_01.png"));

    PubLabel *windowTitleLabel = new PubLabel(titleBar);
    windowTitleLabel->setMinimumWidth(140);
    windowTitleLabel->setFixedHeight(iconw);
    windowTitleLabel->setText(TR::SettingsCenter);
    windowTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    windowTitleLabel->setPointSize(10);

    PubPushButton *closeBtn = new PubPushButton(titleBar);
    closeBtn->setFixedSize(iconw, iconw);
    closeBtn->setBackground(RCC_WRAPPER("hy_main_close_01.png"));
    closeBtn->setBackgroundColorByHovered(QColor(205, 0, 0, 150));
    closeBtn->setBackgroundColorByPressed(QColor(205, 0, 0, 255));
    QObject::connect(closeBtn, &PubPushButton::clicked, q, &AppSettinsWidget::close);

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(logoLabel);
    layout1->addWidget(windowTitleLabel);
    layout1->addStretch();
    layout1->addWidget(closeBtn);
    layout1->setContentsMargins(4, 0, 2, 4);
    titleBar->setLayout(layout1);
    //! [0] titleBar

    //! [1] body
    QWidget *bodyWidget = new QWidget(q);

    bodyTabBar_ = new AppSettingsTabBar(bodyWidget);
    bodyTabBar_->setFixedWidth(92);

    QFrame *vline = new QFrame(bodyWidget);
    vline->setFixedWidth(1);
    G_UISETTIGNS->setWidgetBackgroundColor(vline, QColor(220, 231, 237));

    bodyContentWidget_ = new AppSettingsBodyWidget(bodyWidget);

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(bodyTabBar_);
    layout2->addWidget(vline);
    layout2->addWidget(bodyContentWidget_);
    layout2->setMargin(0);
    layout2->setSpacing(0);
    bodyWidget->setLayout(layout2);
    //! [1] body

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(bodyWidget);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    q->contentWidget()->setLayout(mainLayout);

    QObject::connect(bodyTabBar_, &AppSettingsTabBar::currentIndexChanged, bodyContentWidget_, &AppSettingsBodyWidget::setCurrentIndex);
}
