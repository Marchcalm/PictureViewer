#ifndef APPSETTINGSTABBAR_H
#define APPSETTINGSTABBAR_H

#include <QWidget>

class AppSettingsTabBar : public QWidget
{
    Q_OBJECT
public:
    explicit AppSettingsTabBar(QWidget *parent = nullptr);
    ~AppSettingsTabBar();

signals:
    void currentIndexChanged(int index);

private slots:
    void onButtonClicked();

private:
    class PrivateData;
    PrivateData *d;
};

#endif // APPSETTINGSTABBAR_H
