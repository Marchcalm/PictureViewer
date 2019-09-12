#include "appsettinswidget.h"
#include "pub_pushbutton.h"
#include "pub_label.h"

#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>

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
    QStackedWidget *stackedWidget_;

    void init();
};

AppSettinsWidget::AppSettinsWidget(QWidget *parent) : PubSubWindow(parent),
    d(new PrivateData(this))
{
    d->init();
}

AppSettinsWidget::~AppSettinsWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
    delete d;
}

void AppSettinsWidget::PrivateData::init()
{
    q->setFixedSize(600, 410);
//    q->setWindowFlags(q->windowFlags() | Qt::WindowStaysOnTopHint);
    q->setAttribute(Qt::WA_DeleteOnClose);

    //! [0] titleBar
    int th = 30;
    int iconw = th - 2;
    QWidget *titleBar = new QWidget(q);
    titleBar->setFixedHeight(th);
    QPalette pal(titleBar->palette());
    pal.setColor(QPalette::Window, QColor(140, 140, 140, 150));
    titleBar->setPalette(pal);
    titleBar->setAutoFillBackground(true);

    PubLabel *logoLabel = new PubLabel(titleBar);
    logoLabel->setFixedSize(iconw, iconw);
    logoLabel->setPixmap(QLatin1String(":/images/hy_main_settings_01.png"));

    PubLabel *windowTitleLabel = new PubLabel(titleBar);
    windowTitleLabel->setMinimumWidth(140);
    windowTitleLabel->setFixedHeight(iconw);
    windowTitleLabel->setText(TR::SettingsCenter);
    windowTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    windowTitleLabel->setPointSize(10);

    PubPushButton *closeBtn = new PubPushButton(titleBar);
    closeBtn->setFixedSize(iconw, iconw);
    closeBtn->setBackground(QLatin1String(":/images/hy_main_close_01.png"));
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
    stackedWidget_ = new QStackedWidget(q);
    //! [1] body

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(stackedWidget_);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    q->setLayout(mainLayout);
}
