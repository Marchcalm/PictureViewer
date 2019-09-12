#ifndef MAINTITLEBAR_H
#define MAINTITLEBAR_H

#include <QWidget>

class MainTitleBar : public QWidget
{
    Q_OBJECT
public:
    enum ButtonType {
        BT_FullScreen = 0x01,
        BT_Minimize = 0x02,
        BT_Maximize = 0x03,
        BT_Close = 0x04
    };

    enum ActionType {
        AT_AppSettings = 0x01
    };

    explicit MainTitleBar(QWidget *parent = nullptr);
    ~MainTitleBar() override;

public slots:
    void setTitle(const QString &text);
    void onWindowStateChanged(Qt::WindowStates state);

signals:
    void clicked(int buttonType);
    void actionTriggered(int actionType);

protected:
    void paintEvent(QPaintEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;

private slots:
    void onButtonClicked();
    void onThemeChanged(int type);

private:
    class PrivateData;
    PrivateData *d;
};

#endif // MAINTITLEBAR_H
