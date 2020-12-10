#ifndef PUB_WINDOW_H
#define PUB_WINDOW_H

#include <QWidget>
#include <QHBoxLayout>

class PubWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PubWindow(QWidget *parent = nullptr);

    QWidget *contentWidget() const;
    void setBackgroundColor(const QColor &color);

    QColor windowShadow() const;
    void setWindowShadow(const QColor &c);

signals:
    void windowStateChaned(Qt::WindowStates state);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void changeEvent(QEvent *e) override;

private:
    int calcPosition(const QPoint &pt);
    void setCursorType(int value);
    void setShadowEffect(qreal blurRadius);

private:
    bool isPress_;
    QPoint lastGlobalPos_;
    int cursorPosition_;
    Qt::CursorShape cursorShepe_;
    QWidget *contentWidget_;
    QColor windowShadowColor_;
};

#endif // PUB_WINDOW_H6
