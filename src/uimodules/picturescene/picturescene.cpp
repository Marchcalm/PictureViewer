#include "picturescene.h"
#include "pixmapitem.h"
#include "picturequickviewbutton.h"
#include "uiglobal.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPointer>
#include <QFileDialog>
#include <QTimer>
#include <QDir>

#include <QDebug>

static const qreal kSceneMaxWidth = 10000.0;
static const qreal kSceneMaxHeight = 10000.0;

#define IMAGE_FORMAT_COUNT 23
static const char *kImageFormat[IMAGE_FORMAT_COUNT] = {
    "bmp", "jpg", "jpeg", "jpe", "tbi", "png", "gif",
    "tif", "tiff", "ico", "psd", "psb", "tga", "svg",
    "svgz", "xbm", "xpm", "wdb", "crw", "webp", "wmf",
    "emf", "xmind"
};

class PictureScene::PrivateData
{
public:
    explicit PrivateData(PictureScene *parent) :
        q(parent),
        scene_(nullptr),
        toPreviousButton_(nullptr),
        toNextButton_(nullptr),
        adativeType_(PictureScene::AT_Ratio_1x1),
        fileIndex_(-1),
        rotateAngles_(0)
    {}

    PictureScene *q;
    QGraphicsScene *scene_;
    QPointer<PixmapItem> pixmapItem_;
    PictureQuickViewButton *toPreviousButton_;
    PictureQuickViewButton *toNextButton_;

    QPixmap originalPixmap_;
    PictureScene::AdaptiveType adativeType_;

    QString fileName_;
    QString fileInfo_;
    QStringList imagePathList_;
    int fileIndex_;
    int rotateAngles_;

    void showOriginalPixmap();
    void showRatio1x1Pixmap();
    void setPixmapAlignAlignment(int alignment, QRectF containerRect);
    void flushScene();
    QString transformFileSize(qint64 fileSize);
    void searchImageFiles(const QString &dirPath);
    bool compareImageSuffix(const QString &suffix);
    void updateFileInfo(const QString &fileName, bool isSearch);
    void setQuickViewButtonVisible(bool visible);
    void rotate(int angles);
};

PictureScene::PictureScene(QWidget *parent) : QGraphicsView(parent),
    d(new PrivateData(this))
{  
    // 跟踪鼠标
    setMouseTracking(true);

    // 允许鼠标拖放
    setAcceptDrops(true);

    // 隐藏滚动条
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 设置背景颜色和边框
    setStyleSheet(QString("background: %1; border: 0px;").arg(G_UISETTIGNS->themeColor().name()));

    QObject::connect(G_UISETTIGNS, &UiGlobalSettings::themeChanged, [&](){
        QColor c = G_UISETTIGNS->themeColor();
        this->setStyleSheet(QString("background: %1; border: 0px;").arg(c.name()));
        this->setBackgroundBrush(c);
    });
}

PictureScene::~PictureScene()
{
    delete d;
}

bool PictureScene::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Images (*.png *.xpm *.jpg);; All Files (*.*)"));
    if (fileName.isEmpty())
        return false;

    showPicture(fileName);
    d->flushScene();

    return true;
}

QString PictureScene::fileInfo() const
{
    return d->fileInfo_;
}

void PictureScene::showPicture(const QPixmap &pixmap, AdaptiveType type)
{
    if (!d->scene_) {
        // 设置场景
        d->scene_ = new QGraphicsScene(this);
        setScene(d->scene_);
        setSceneRect(0.0, 0.0, kSceneMaxWidth, kSceneMaxHeight);
        setBackgroundBrush(G_UISETTIGNS->themeColor());
    }

    if (d->pixmapItem_) {
        d->scene_->removeItem(d->pixmapItem_);
        delete d->pixmapItem_.data();
    }

    if (d->pixmapItem_.isNull()) {
        d->pixmapItem_ = new PixmapItem;
        d->pixmapItem_->setFlag(QGraphicsItem::ItemIsMovable);
    }

    d->scene_->addItem(d->pixmapItem_);
    d->originalPixmap_ = pixmap;
    d->pixmapItem_->setPixmap(pixmap);

    setAdaptiveType(type);
    switch (d->adativeType_) {
    case PictureScene::AT_Original:
        d->showOriginalPixmap();
        break;
    case PictureScene::AT_Ratio_1x1:
        d->showRatio1x1Pixmap();
        break;
    }

    update();
}

void PictureScene::showPicture(const QString &fileName, AdaptiveType type)
{
    d->fileName_ = fileName;
    QPixmap pix = QPixmap::fromImage(QImage(fileName));
    showPicture(pix, type);
    d->updateFileInfo(fileName, true);
}

void PictureScene::adaptiveToggle()
{
    if (!d->pixmapItem_)
        return;

    switch (d->adativeType_) {
    case PictureScene::AT_Original:
        showPicture(d->originalPixmap_, PictureScene::AT_Ratio_1x1);
        break;
    case PictureScene::AT_Ratio_1x1:
        showPicture(d->originalPixmap_, PictureScene::AT_Original);
        break;
    }
}

void PictureScene::zoomIn()
{
    if (!d->pixmapItem_ || d->pixmapItem_->scale() > 3.2)
        return;

    QRectF previousRect = d->pixmapItem_->sceneBoundingRect();
    d->pixmapItem_->setScale(d->pixmapItem_->scale() * 1.1);
    d->pixmapItem_->setPos(d->pixmapItem_->x() - (d->pixmapItem_->sceneBoundingRect().width() - previousRect.width()) / 2,
                           d->pixmapItem_->y() - (d->pixmapItem_->sceneBoundingRect().height() - previousRect.height()) / 2);
}

void PictureScene::zoomOut()
{
    if (!d->pixmapItem_)
        return;

    QRectF previousRect = d->pixmapItem_->sceneBoundingRect();
    d->pixmapItem_->setScale(d->pixmapItem_->scale() * 0.9);
    d->pixmapItem_->setPos(d->pixmapItem_->x() - (d->pixmapItem_->sceneBoundingRect().width() - previousRect.width()) / 2,
                           d->pixmapItem_->y() - (d->pixmapItem_->sceneBoundingRect().height() - previousRect.height()) / 2);
}

void PictureScene::rotateLeft()
{
    if (!d->pixmapItem_)
        return;

    int angles = static_cast<int>(d->pixmapItem_->rotation()) - 90;
    if (angles < -270)
        angles = 0;

    d->rotate(angles);
}

void PictureScene::rotateRight()
{
    if (!d->pixmapItem_)
        return;

    int angles = static_cast<int>(d->pixmapItem_->rotation()) + 90;
    if (angles > 270)
        angles = 0;

    d->rotate(angles);
}

void PictureScene::setAdaptiveType(PictureScene::AdaptiveType type)
{
    if (d->adativeType_ != type) {
        d->adativeType_ = type;
        emit adaptiveTypeChanged(type);
    }
}

void PictureScene::toPrevious()
{
    if (d->imagePathList_.isEmpty())
        return;

    if (--d->fileIndex_ < 0) {
        d->fileIndex_ = d->imagePathList_.size() - 1;
    }

    QString fileName = d->imagePathList_.at(d->fileIndex_);
    showPicture(fileName);
    d->updateFileInfo(fileName, false);
}

void PictureScene::toNext()
{
    if (d->imagePathList_.isEmpty())
        return;

    if (++d->fileIndex_ > d->imagePathList_.size() - 1) {
        d->fileIndex_ = 0;
    }

    QString fileName = d->imagePathList_.at(d->fileIndex_);
    showPicture(fileName);
    d->updateFileInfo(fileName, false);
}

void PictureScene::resizeEvent(QResizeEvent *)
{
    if (d->pixmapItem_) {
        showPicture(d->originalPixmap_, d->adativeType_);
        d->rotate(d->rotateAngles_);
    }

    if (d->toPreviousButton_) {
        d->toPreviousButton_->move(30, (height() - d->toPreviousButton_->height())/2);
    }

    if (d->toNextButton_) {
        d->toNextButton_->move(width() - 30 - d->toNextButton_->width(), (height() - d->toNextButton_->height())/2);
    }
}

void PictureScene::wheelEvent(QWheelEvent *e)
{
    if (e->angleDelta().y() > 0) {
        zoomOut();
    } else {
        zoomIn();
    }
}

void PictureScene::mouseMoveEvent(QMouseEvent *e)
{
    if (!d->scene_) {
        e->ignore();
    } else {
        int aw = width()/5;
        if (e->x() < aw || e->x() > width()-aw) {
            d->setQuickViewButtonVisible(true);
        } else {
            d->setQuickViewButtonVisible(false);
        }

        QList<QGraphicsItem*> items = d->scene_->items(e->pos());
        if (items.isEmpty()) {
            setCursor(Qt::ArrowCursor);
            e->ignore();
        } else {
            setCursor(Qt::OpenHandCursor);
            QGraphicsView::mouseMoveEvent(e);
        }
    }
}

void PictureScene::mousePressEvent(QMouseEvent *e)
{
    if (!d->scene_) {
        e->ignore();
    } else {
        QList<QGraphicsItem*> items = d->scene_->items(e->pos());
        if (items.isEmpty()) {
            e->ignore();
        } else {
            setCursor(Qt::ClosedHandCursor);
            QGraphicsView::mousePressEvent(e);
        }
    }
}

void PictureScene::mouseReleaseEvent(QMouseEvent *e)
{
    if (!d->scene_) {
        e->ignore();
    } else {
        QList<QGraphicsItem*> items = d->scene_->items(e->pos());
        if (items.isEmpty()) {
            e->ignore();
        } else {
            setCursor(Qt::OpenHandCursor);
            QGraphicsView::mouseReleaseEvent(e);
        }
    }
}

void PictureScene::dragEnterEvent(QDragEnterEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    if (!urls.isEmpty()) {
        QString fileName = urls.at(0).fileName();
        if (d->compareImageSuffix(fileName.mid(fileName.lastIndexOf(".")+1))) {
            e->acceptProposedAction();
        } else {
            e->ignore();
        }
    } else {
        QGraphicsView::dragEnterEvent(e);
    }
}

void PictureScene::dropEvent(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    if (!urls.isEmpty()) {
        QString filePath = e->mimeData()->urls().at(0).toLocalFile();
        showPicture(filePath);
    } else {
        QGraphicsView::dropEvent(e);
    }
}

void PictureScene::PrivateData::showOriginalPixmap()
{
#if 1
    qreal cx = (q->width() - pixmapItem_->sceneBoundingRect().width()) / 2.0;
    qreal cy = (q->height() - pixmapItem_->sceneBoundingRect().height()) / 2.0;
    pixmapItem_->setPos(QPointF(cx, cy));
#else
    // [0] 根据图片大小调整scene显示位置
    QPointF originalPos(0.0, 0.0);
    bool isOutWidth = originalPixmap_.width() > q->width();
    bool isOutHeight = originalPixmap_.height() > q->height();
    if (isOutWidth) {
        originalPos.setX(q->sceneRect().width()/2);
    }
    if (isOutHeight) {
        originalPos.setY(q->sceneRect().height()/2);
    }
    q->centerOn(originalPos);
    // [0]

    // [1] 图片居中显示
    if (originalPixmap_.width() < q->sceneRect().width() && isOutWidth) {
        setPixmapAlignAlignment(Qt::AlignHCenter, q->sceneRect());
    } else {
        setPixmapAlignAlignment(Qt::AlignHCenter, q->rect());
    }

    if (originalPixmap_.height() < q->sceneRect().height() && isOutHeight) {
        setPixmapAlignAlignment(Qt::AlignVCenter, q->sceneRect());
    } else {
        setPixmapAlignAlignment(Qt::AlignVCenter, q->rect());
    }
    // [1]
#endif
}

void PictureScene::PrivateData::showRatio1x1Pixmap()
{
    if (pixmapItem_->sceneBoundingRect().width() > q->width() || pixmapItem_->sceneBoundingRect().height() > q->height()) {
        qreal ratio = pixmapItem_->sceneBoundingRect().width() / pixmapItem_->sceneBoundingRect().height();
        if (ratio > 1.0) {
            pixmapItem_->setScale(pixmapItem_->scale() * (static_cast<qreal>(q->width()) / originalPixmap_.width() * 0.92));
        } else {
            pixmapItem_->setScale(pixmapItem_->scale() * (static_cast<qreal>(q->height()) / originalPixmap_.height() * 0.92));
        }
    }

    q->centerOn(QPointF(0.0, 0.0));
    setPixmapAlignAlignment(Qt::AlignCenter, q->rect());
}

void PictureScene::PrivateData::setPixmapAlignAlignment(int alignment, QRectF containerRect)
{
    if (!pixmapItem_)
        return;

    switch (alignment) {
    case Qt::AlignHCenter:
        pixmapItem_->setPos((containerRect.width()-pixmapItem_->sceneBoundingRect().width())/2, pixmapItem_->y());
        break;
    case Qt::AlignVCenter:
        pixmapItem_->setPos(pixmapItem_->x(), (containerRect.height()-pixmapItem_->sceneBoundingRect().height())/2);
        break;
    case Qt::AlignCenter:
        pixmapItem_->setPos((containerRect.width()-pixmapItem_->sceneBoundingRect().width())/2,
                            (containerRect.height()-pixmapItem_->sceneBoundingRect().height())/2);
        break;
    }
}

void PictureScene::PrivateData::flushScene()
{
    QTimer::singleShot(10, [&](){
        scene_->update();
    });
}

QString PictureScene::PrivateData::transformFileSize(qint64 fileSize)
{
    if (fileSize < 1024) {
        return QString::number(fileSize) + QLatin1String(" B");
    }
    else if (fileSize >= 1024 && fileSize < 1024 * 1024) {
        return QString::number(fileSize / 1024) + QLatin1String(" KB");
    }
    else {
        qint64 mNum = fileSize / (1024 * 1024);
        int kbNum = fileSize % (1024 * 1024) / 1024;
        QString kbStr;
        if (kbNum == 0) {
            kbStr = QLatin1String("00");
        }
        else if (kbNum > 0 && kbNum < 10) {
            kbStr = QString("0%1").arg(kbNum);
        }
        else {
            kbStr = QString::number(kbNum).left(2);
        }
        return QString::number(mNum) + QLatin1String(".") + kbStr + QLatin1String(" M");
    }
}

void PictureScene::PrivateData::searchImageFiles(const QString &dirPath)
{
    QDir dir(dirPath);
    if(!dir.exists())
        return;

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList fileInfoList = dir.entryInfoList();
    if(fileInfoList.isEmpty())
        return;

    imagePathList_.clear();
    for(int i = 0, j = fileInfoList.count(); i < j; ++i) {
        QFileInfo fileInfo = fileInfoList.at(i);
        QString suffix = fileInfo.suffix();
        if(compareImageSuffix(suffix)) {
            imagePathList_.append(fileInfo.absoluteFilePath());
        }
    }
}

bool PictureScene::PrivateData::compareImageSuffix(const QString &suffix)
{
    for (int i = 0; i < IMAGE_FORMAT_COUNT; ++i) {
        if(QString::compare(suffix, kImageFormat[i], Qt::CaseInsensitive) == 0) {
            return true;
        }
    }

    return false;
}

void PictureScene::PrivateData::updateFileInfo(const QString &fileName, bool isSearch)
{
    fileInfo_.clear();
    fileInfo_.append(fileName.mid(fileName.lastIndexOf("/") + 1));
    fileInfo_.append(QString("(%1, %2x%3px)").arg(transformFileSize(QFile(fileName).size()))
                     .arg(originalPixmap_.width()).arg(originalPixmap_.height()));

    if (isSearch) {
        searchImageFiles(fileName.mid(0, fileName.lastIndexOf("/")));
        fileIndex_ = imagePathList_.indexOf(fileName);
    }

    fileInfo_.append(QString(" - %1/%2").arg(fileIndex_+1).arg(imagePathList_.size()));
    emit q->fileInfoChanged(fileInfo_);
}

void PictureScene::PrivateData::setQuickViewButtonVisible(bool visible)
{
    if (visible) {
        if (!toPreviousButton_) {
            toPreviousButton_ = new PictureQuickViewButton(q);
            toPreviousButton_->setBackground(QLatin1String(":/images/hy_ps_toprevious_XL_01.png"));
            toPreviousButton_->setAdaptiveType(PubPushButton::AT_Image);
            QObject::connect(toPreviousButton_, &PictureQuickViewButton::clicked, [&]() { q->toPrevious(); });
        }
        toPreviousButton_->requestShow();
        if (!toPreviousButton_->isHidden()) {
            toPreviousButton_->move(30, (q->height() - toPreviousButton_->height())/2);
        }

        if (!toNextButton_) {
            toNextButton_ = new PictureQuickViewButton(q);
            toNextButton_->setBackground(QLatin1String(":/images/hy_ps_tonext_XL_01.png"));
            toNextButton_->setAdaptiveType(PubPushButton::AT_Image);
            QObject::connect(toNextButton_, &PictureQuickViewButton::clicked, [&]() { q->toNext(); });
        }
        toNextButton_->requestShow();
        if (!toNextButton_->isHidden()) {
            toNextButton_->move(q->width() - 30 - toNextButton_->width(), (q->height() - toNextButton_->height())/2);
        }

    } else {
        if (toPreviousButton_ && !toPreviousButton_->isHidden()) {
            toPreviousButton_->requestHide();
        }
        if (toNextButton_ && !toNextButton_->isHidden()) {
            toNextButton_->requestHide();
        }
    }
}

void PictureScene::PrivateData::rotate(int angles)
{
    if (!pixmapItem_)
        return;

    rotateAngles_ = angles;

    if (pixmapItem_->scale() != 1.0) {
        pixmapItem_->setScale(1.0);
        showOriginalPixmap();
    }

    // 设置旋转中心
    QRectF boundingRect = pixmapItem_->boundingRect();
    pixmapItem_->setTransformOriginPoint(boundingRect.center());

    pixmapItem_->setRotation(angles);

    if (angles == 90 || angles == -270) {
        if (boundingRect.height() > q->height()) {
            qreal hhRatio = q->height() / boundingRect.width();
            pixmapItem_->setScale(hhRatio);
            q->centerOn(QPointF(0.0, 0.0));
            pixmapItem_->setTransformOriginPoint(QPointF(0.0, 0.0));
            QRectF sceneBoundingRect = pixmapItem_->sceneBoundingRect();
            qreal cx = sceneBoundingRect.width() + (q->width() - sceneBoundingRect.width()) / 2.0;
            qreal cy = 0.0;
            pixmapItem_->setPos(cx, cy);
        }
    }
    else if (angles == 180 || angles == -180) {
        if (boundingRect.width() > q->width() || boundingRect.height() > q->height()) {
            showRatio1x1Pixmap();
            pixmapItem_->setTransformOriginPoint(QPointF(0.0, 0.0));
            QRectF sceneBoundingRect = pixmapItem_->sceneBoundingRect();
            qreal cx = sceneBoundingRect.width() + (q->width() - sceneBoundingRect.width()) / 2.0;
            qreal cy = sceneBoundingRect.height() + (q->height() - sceneBoundingRect.height()) / 2.0;
            pixmapItem_->setPos(QPointF(cx, cy));
        }
    }
    else if (angles == 270 || angles == -90) {
        if (boundingRect.height() > q->height()) {
            qreal hhRatio = q->height() / boundingRect.width();
            pixmapItem_->setScale(hhRatio);
            q->centerOn(QPointF(0.0, 0.0));
            pixmapItem_->setTransformOriginPoint(QPointF(0.0, 0.0));
            QRectF sceneBoundingRect = pixmapItem_->sceneBoundingRect();
            qreal cx = (q->width() - sceneBoundingRect.width()) / 2.0;
            qreal cy = sceneBoundingRect.height();
            pixmapItem_->setPos(cx, cy);
        }
    }
    else if (angles == 0) {
        if (boundingRect.width() > q->width() || boundingRect.height() > q->height()) {
            showRatio1x1Pixmap();
            pixmapItem_->setTransformOriginPoint(QPointF(0.0, 0.0));
            QRectF sceneBoundingRect = pixmapItem_->sceneBoundingRect();
            qreal cx = (q->width() - sceneBoundingRect.width()) / 2.0;
            qreal cy = (q->height() - sceneBoundingRect.height()) / 2.0;
            pixmapItem_->setPos(QPointF(cx, cy));
        }
    }

    // 保存
    QMatrix matrix;
    matrix.rotate(angles);

    QPixmap pix = pixmapItem_->pixmap();
    pix = pix.transformed(matrix, Qt::SmoothTransformation);
    pix.save(fileName_);
}
