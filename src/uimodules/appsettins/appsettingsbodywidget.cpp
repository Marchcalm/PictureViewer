#include "appsettingsbodywidget.h"
#include "appsettingsbodywidget/asb_basicsettingswidget.h"

#include <QStackedWidget>

class AppSettingsBodyWidget::PrivateData
{
public:
    explicit PrivateData(AppSettingsBodyWidget *parent) :
        q(parent), basicSettingsWidget_(nullptr)
    {}

    AppSettingsBodyWidget *q;

    QStackedWidget *stackedWidget_;
    AsbBasicSettingsWidget *basicSettingsWidget_;

    void init();
};

AppSettingsBodyWidget::AppSettingsBodyWidget(QWidget *parent) : QWidget(parent),
    d(new PrivateData(this))
{
    d->init();
}

AppSettingsBodyWidget::~AppSettingsBodyWidget()
{
    delete d;
}

void AppSettingsBodyWidget::setCurrentIndex(int index)
{
    switch (index) {
    case 0:
        d->stackedWidget_->setCurrentWidget(d->basicSettingsWidget_);
        break;
    case 1:
        break;
    default:
        break;
    }
}

void AppSettingsBodyWidget::resizeEvent(QResizeEvent *)
{
    d->stackedWidget_->setGeometry(this->rect());
}

void AppSettingsBodyWidget::PrivateData::init()
{
    stackedWidget_ = new QStackedWidget(q);

    basicSettingsWidget_ = new AsbBasicSettingsWidget(q);
    stackedWidget_->addWidget(basicSettingsWidget_);
}
