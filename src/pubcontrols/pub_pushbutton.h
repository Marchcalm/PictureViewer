#ifndef PUB_PUSHBUTTON_H
#define PUB_PUSHBUTTON_H

#include <QAbstractButton>

class PubPushButton : public QAbstractButton
{
    Q_OBJECT
public:
    enum AadptiveType { AT_NoAdaptive = 0, AT_Button = 1, AT_Image = 2};

    explicit PubPushButton(QWidget *parent = nullptr);
    ~PubPushButton() override;

    void setBackground(const QString &normal, const QString &press = QString(), const QString &check = QString());

    void setAdaptiveType(AadptiveType type);

    QColor backgroundColorByHovered() const;
    void setBackgroundColorByHovered(const QColor &c);

    QColor backgroundColorByPressed() const;
    void setBackgroundColorByPressed(const QColor &c);

    QString text() const;
    void setText(const QString &text, bool translatable = true);

    void setTextColor(const QColor &normal, const QColor &hovered = QColor(), const QColor &pressed = QColor());

protected:
    void paintEvent(QPaintEvent *) override;
    bool event(QEvent *e) override;

private:
    void drawBackground(int pixmapIndex, QPainter *painter);

private:
    QPixmap backgroundPixmaps_[3];
    uint isEnter_ : 1;
    uint adaptiveType_ : 2;
    QColor backgroundColorByHovered_;
    QColor backgroundColorByPressed_;
    QString text_;
    bool translatable_;
    QColor textNormalColor_;
    QColor textHoveredColor_;
    QColor textPressedColor_;
};

#endif // PUB_PUSHBUTTON_H
