#include "appsettingsbodywidget.h"
#include "appsettingsbodywidget/asb_basicsettingswidget.h"
#include "appsettingsbodywidget/asb_powerpointwidget.h"

#include <QStackedWidget>

class AppSettingsBodyWidget::PrivateData
{
public:
    explicit PrivateData(AppSettingsBodyWidget *parent) :
        q(parent), basicSettingsWidget_(nullptr), PowerPointWidget_(nullptr)
    {}

    AppSettingsBodyWidget *q;

    QStackedWidget *stackedWidget_;
    AsbBasicSettingsWidget *basicSettingsWidget_;
    AsbPowerPointWidget *PowerPointWidget_;

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
        if (!d->PowerPointWidget_) {
            d->PowerPointWidget_ = new AsbPowerPointWidget(this);
            d->stackedWidget_->addWidget(d->PowerPointWidget_);
        }
        d->stackedWidget_->setCurrentWidget(d->PowerPointWidget_);
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
