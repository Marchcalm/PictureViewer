#ifndef PUB_SUBWINDOW_H
#define PUB_SUBWINDOW_H

#include <QWidget>

class PubSubWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PubSubWindow(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    bool isPress_;
    QPoint lastGlobalPos_;
};

#endif // PUB_SUBWINDOW_H
