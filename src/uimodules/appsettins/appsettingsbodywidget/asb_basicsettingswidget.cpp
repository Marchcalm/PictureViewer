#include "asb_basicsettingswidget.h"
#include "pub_label.h"
#include "uiglobal.h"
#include "mainwindow.h"

#include <QPushButton>
#include <QColorDialog>

class AsbBasicSettingsWidget::PrivateData
{
public:
    explicit PrivateData(AsbBasicSettingsWidget *parent) :
        q(parent)
    {}

    AsbBasicSettingsWidget *q;

    PubLabel *windowShadowLabel_;
    QPushButton *windowShadowBtn_;

    void init();
};

AsbBasicSettingsWidget::AsbBasicSettingsWidget(QWidget *parent) : QWidget(parent),
    d(new PrivateData(this))
{
    d->init();
}

AsbBasicSettingsWidget::~AsbBasicSettingsWidget()
{
    delete d;
}

void AsbBasicSettingsWidget::resizeEvent(QResizeEvent *)
{
    d->windowShadowLabel_->setGeometry(15, 15, 120, 25);
    d->windowShadowBtn_->setGeometry(180, 15, 25, 25);
}

void AsbBasicSettingsWidget::PrivateData::init()
{
    windowShadowLabel_ = new PubLabel(q);
    windowShadowLabel_->setText("Window shadow");

    windowShadowBtn_ = new QPushButton(q);
    windowShadowBtn_->setStyleSheet(QString("background: %1").arg(UiGlobalSettings::obj()->mainWindow()->windowShadow().name()));
    QObject::connect(windowShadowBtn_, &QPushButton::clicked, [&]() {
        QColor c = QColorDialog::getColor();
        if (c.isValid()) {
            windowShadowBtn_->setStyleSheet(QString("background: %1").arg(c.name()));
            UiGlobalSettings::obj()->mainWindow()->setWindowShadow(c);
        }
    });
}
