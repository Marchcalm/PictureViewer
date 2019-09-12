#include "pixmapitem.h"
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

class PixmapItem::PrivateData
{
public:
    PrivateData() : isPressed(false)
    {}

    bool isPressed;
};

PixmapItem::PixmapItem(QGraphicsItem *parent) : QGraphicsPixmapItem(parent),
    d(new PrivateData)
{

}

PixmapItem::~PixmapItem()
{
    delete d;
}

void PixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
//    if (!pixmap().isNull()) {
//        if (d->isPressed) {
//            setCursor(Qt::ClosedHandCursor);
//        } else {
//            setCursor(Qt::OpenHandCursor);
//        }
//    }
    QGraphicsPixmapItem::mouseMoveEvent(e);
}

void PixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
//    d->isPressed = true;
//    if (!pixmap().isNull()) {
//        setCursor(Qt::ClosedHandCursor);
//    }
    QGraphicsPixmapItem::mousePressEvent(e);
}

void PixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
//    d->isPressed = false;
//    if (!pixmap().isNull()) {
//        setCursor(Qt::OpenHandCursor);
//    }
    QGraphicsPixmapItem::mouseReleaseEvent(e);
}

