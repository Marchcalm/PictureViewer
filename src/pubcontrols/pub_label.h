#ifndef PUB_LABEL_H
#define PUB_LABEL_H

#include <QWidget>

class PubLabel : public QWidget
{
    Q_OBJECT
public:
    explicit PubLabel(QWidget *parent = nullptr);
    ~PubLabel() override;

    void setText(const QString &text, bool translatable = true);
    QString text() const;

    void setPixmap(const QPixmap &pixmap);
    void setPixmap(const QString &fileName);
    QPixmap pixmap() const;

    void setPointSize(int size);
    void setPixelSize(int size);

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment type);

    void setTextColor(const QColor &color);

    void setBorder(int width, QColor c = Qt::red);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QString text_;
    QPixmap backgroundPixmap_;
    Qt::Alignment alignment_;
    QColor textColor_;
    uint translatable_ : 1;
    int borderWidth_;
    QColor borderColor_;
};

#endif // PUB_LABEL_H
