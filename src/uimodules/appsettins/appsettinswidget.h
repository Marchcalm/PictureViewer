#ifndef APPSETTINSWIDGET_H
#define APPSETTINSWIDGET_H

#include "pub_subwindow.h"

class AppSettinsWidget : public PubSubWindow
{
    Q_OBJECT
public:
    explicit AppSettinsWidget(QWidget *parent = nullptr);
    ~AppSettinsWidget() override;

private:
    class PrivateData;
    PrivateData *d;
};

#endif // APPSETTINSWIDGET_H
