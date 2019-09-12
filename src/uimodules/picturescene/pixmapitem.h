#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include <QGraphicsPixmapItem>

class QGraphicsSceneMouseEvent;

class PixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit PixmapItem(QGraphicsItem *parent = nullptr);
    ~PixmapItem() override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;

private:
    class PrivateData;
    PrivateData *d;
};

#endif // PIXMAPITEM_H
