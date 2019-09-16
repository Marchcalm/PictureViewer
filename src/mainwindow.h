#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pub_window.h"

class MainWindow : public PubWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onMainTitleBarClicked(int buttonType);
    void onMainTitleBarActionTriggered(int actionType);
    void onPictureToolBarClicked(int buttonType);
    void onThemeChanged(int type);
    void onWindowStateChanged(Qt::WindowStates state);

protected:
    void keyPressEvent(QKeyEvent *) override;
    void changeEvent(QEvent *) override;

private:
    class PrivateData;
    PrivateData *d;
};

#endif // MAINWINDOW_H
