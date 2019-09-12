#ifndef PICTURETOOLBAR_H
#define PICTURETOOLBAR_H

#include <QWidget>

class PictureToolBar : public QWidget
{
    Q_OBJECT
public:
    enum ButtonType {
        BT_OpenFile = 0x01,
        BT_Edit = 0x02,
        BT_Adaptive = 0x03,
        BT_ZoomIn = 0x04,
        BT_ZoomOut = 0x05,
        BT_ToPrevious = 0x06,
        BT_ToNext = 0x07,
        BT_RotateLeft = 0x08,
        BT_RotateRight = 0x09
    };

    explicit PictureToolBar(QWidget *parent = nullptr);
    ~PictureToolBar() override;

signals:
    void clicked(int buttonType);

public slots:
    void setAdaptiveType(int type);

protected:
    void paintEvent(QPaintEvent *) override;

private slots:
    void onButtonClicked();
    void onThemeChanged(int type);

private:
    class PrivateData;
    PrivateData *d;
};

#endif // PICTURETOOLBAR_H
