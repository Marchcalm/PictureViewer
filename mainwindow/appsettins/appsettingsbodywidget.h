#ifndef APPSETTINGSBODYWIDGET_H
#define APPSETTINGSBODYWIDGET_H

#include <QWidget>

class AppSettingsBodyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AppSettingsBodyWidget(QWidget *parent = nullptr);
    ~AppSettingsBodyWidget() override;

public slots:
    void setCurrentIndex(int index);

protected:
    void resizeEvent(QResizeEvent *) override;

private:
    class PrivateData;
    PrivateData *d;
};

#endif // APPSETTINGSBODYWIDGET_H
