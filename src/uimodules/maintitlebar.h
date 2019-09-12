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

    explicit MainTitleBar(QWidget *parent = nullptr);
    ~MainTitleBar() override;

    void setTitle(const QString &text);
    void handleWindowState(Qt::WindowStates state);

signals:
    void clicked(int buttonType);

protected:
    virtual void paintEvent(QPaintEvent *) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *e) override;

private slots:
    void onButtonClicked();

private:
    class PrivateData;
    PrivateData *d;
};

#endif // MAINTITLEBAR_H
