#ifndef PUB_SUBWINDOW_H
#define PUB_SUBWINDOW_H

#include <QWidget>

class PubSubWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PubSubWindow(QWidget *parent = nullptr);

    QWidget *contentWidget() const;
    void setBackgroundColor(const QColor &color);

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    bool isPress_;
    QPoint lastGlobalPos_;
    QWidget *contentWidget_;
};

#endif // PUB_SUBWINDOW_H
