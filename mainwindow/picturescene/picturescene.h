#ifndef PICTURESCENE_H
#define PICTURESCENE_H

#include <QGraphicsView>

class PictureScene : public QGraphicsView
{
    Q_OBJECT
public:
    enum AdaptiveType {
        AT_Original = 0x01,
        AT_Ratio_1x1 = 0x02
    };

    explicit PictureScene(QWidget *parent = nullptr);
    ~PictureScene() override;

    /**
     * @brief 通过文件对话框打开
     */
    bool openFile();

    /**
     * @brief 当前打开文件的信息（图片大小，分辨率）
     */
    QString fileInfo() const;

    /**
     * @brief 重载showPicture
     * @param QPixmap 图片
     * @param type 图片显示比例类型，默认按1:1缩放显示
     */
    void showPicture(const QPixmap &pixmap, PictureScene::AdaptiveType type = PictureScene::AT_Ratio_1x1);

    /**
     * @brief 重载showPicture
     */
    void showPicture(const QString &fileName, PictureScene::AdaptiveType type = PictureScene::AT_Ratio_1x1);

    /**
     * @brief 在图片适应窗口和原图显示之间切换
     */
    void adaptiveToggle();

    /**
     * @brief 放大
     */
    void zoomIn();

    /**
     * @brief 缩小
     */
    void zoomOut();

    /**
     * @brief 向左旋转
     */
    void rotateLeft();

    /**
     * @brief 向右旋转
     */
    void rotateRight();

    /**
     * @brief 设置图片自适应类型
     * @param type 包含有原图，1:1缩放
     */
    void setAdaptiveType(PictureScene::AdaptiveType type);

    /**
     * @brief 上一张
     */
    void toPrevious();

    /**
     * @brief 下一张
     */
    void toNext();

signals:
    void fileInfoChanged(const QString &fileInfo);
    void adaptiveTypeChanged(int);
    void twoSidesEntered();

protected:
    void resizeEvent(QResizeEvent *) override;
    void wheelEvent(QWheelEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void dragEnterEvent(QDragEnterEvent *) override;
    void dropEvent(QDropEvent *) override;

private:
    class PrivateData;
    PrivateData *d;
};

#endif // PICTURESCENE_H
