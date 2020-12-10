#ifndef ASB_BASICSETTINGSWIDGET_H
#define ASB_BASICSETTINGSWIDGET_H

#include <QWidget>

class AsbBasicSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AsbBasicSettingsWidget(QWidget *parent = nullptr);
    ~AsbBasicSettingsWidget() override;

protected:
    void resizeEvent(QResizeEvent *) override;

private:
    class PrivateData;
    PrivateData *d;
};

#endif // ASB_BASICSETTINGSWIDGET_H
