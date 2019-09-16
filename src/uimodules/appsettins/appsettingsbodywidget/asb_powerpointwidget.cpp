#include "asb_powerpointwidget.h"
#include "uiglobal.h"

AsbPowerPointWidget::AsbPowerPointWidget(QWidget *parent) : QWidget(parent)
{
    G_UISETTIGNS->setWidgetBackgroundColor(this, Qt::green);
}
