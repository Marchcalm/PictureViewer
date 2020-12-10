#include "pub_label.h"

#include <QPainter>

PubLabel::PubLabel(QWidget *parent) :
    QWidget(parent), alignment_(Qt::AlignCenter), textColor_(Qt::black),
    borderWidth_(0), borderColor_(Qt::red)
{
}

PubLabel::~PubLabel()
{

}

void PubLabel::setText(const QString &text, bool translatable)
{
    if (text_ != text) {
        text_ = text;
        translatable_ = translatable;
        update();
    }
}

QString PubLabel::text() const
{
    return text_;
}

Qt::Alignment PubLabel::alignment() const
{
    return alignment_;
}

void PubLabel::setAlignment(Qt::Alignment type)
{
    alignment_ = type;
    update();
}

void PubLabel::setTextColor(const QColor &color)
{
    textColor_ = color;
    update();
}

void PubLabel::setBorder(int width, QColor c)
{
    borderWidth_ = width;
    borderColor_ = c;
    update();
}

void PubLabel::setPixmap(const QPixmap &pixmap)
{
    backgroundPixmap_ = pixmap;
    update();
}

void PubLabel::setPixmap(const QString &fileName)
{
    backgroundPixmap_ = QPixmap::fromImage(QImage(fileName));
    update();
}

QPixmap PubLabel::pixmap() const
{
    return backgroundPixmap_;
}

void PubLabel::setPointSize(int size)
{
    QFont f(this->font());
    f.setPointSize(size);
    this->setFont(f);
    update();
}

void PubLabel::setPixelSize(int size)
{
    QFont f(this->font());
    f.setPixelSize(size);
    this->setFont(f);
    update();
}

void PubLabel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (!backgroundPixmap_.isNull()) {
        painter.setRenderHints(QPainter::Antialiasing);
        painter.drawPixmap((width() - backgroundPixmap_.width()) >> 1,
                           (height() - backgroundPixmap_.height()) >> 1,
                           backgroundPixmap_);
    }

    if (!text_.isEmpty()) {
        painter.setPen(textColor_);

        QString tmpText;
        if (translatable_) {
            tmpText = QObject::tr(text_.toUtf8().constData());
        } else {
            tmpText = text_;
        }

        QFontMetrics fontMetrics(font());
        int padding = 2;
        QRect textRect(padding, padding, width() - padding * 2, height() - padding * 2);
        int textWidth = fontMetrics.horizontalAdvance(tmpText);
        if (textWidth > textRect.width()) {
            tmpText = fontMetrics.elidedText(tmpText, Qt::ElideRight, textRect.width());
        }
        painter.drawText(textRect, static_cast<int>(alignment_), tmpText);
    }

    if (borderWidth_ > 0) {
        painter.setPen(QPen(borderColor_, borderWidth_));
        painter.drawRect(borderWidth_, borderWidth_, width()-borderWidth_*2, height()-borderWidth_*2);
    }
}
