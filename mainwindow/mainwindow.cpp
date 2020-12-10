#include "mainwindow.h"
#include "maintitlebar.h"
#include "picturescene/picturescene.h"
#include "picturescene/picturetoolbar.h"
#include "appsettins/appsettinswidget.h"
#include "pub_pushbutton.h"
#include "uiglobal.h"

#include <QVBoxLayout>
#include <QApplication>
#include <QTimer>
#include <QKeyEvent>
#include <QPointer>
#include <QEvent>

class MainWindow::PrivateData
{
public:
    explicit PrivateData(MainWindow *parent) :
        q(parent), exitFullScreenBtn_(nullptr),
        windowOverlay_(nullptr)
    {}

    MainWindow *q;

    MainTitleBar *titleBar_;
    PictureScene *pictureScene_;
    PictureToolBar *pictureToolBar_;
    PubPushButton *exitFullScreenBtn_;
    QWidget *windowOverlay_;
    QPointer<AppSettinsWidget> appSettingsWidget_;

    void init();
    void setWindowOverlayVisible(bool visible);
    void showAppSettingsWidget();
};

MainWindow::MainWindow(QWidget *parent) :
    PubWindow(parent),
    d(new PrivateData(this))
{
    d->init();
}

MainWindow::~MainWindow()
{
    delete d;
}

void MainWindow::onMainTitleBarClicked(int buttonType)
{
    switch (buttonType) {
    case MainTitleBar::BT_FullScreen:       
        if (windowState() == Qt::WindowFullScreen) {
            showNormal();
        } else {
            showFullScreen();
        }
        break;
    case MainTitleBar::BT_Minimize:
        showMinimized();
        break;
    case MainTitleBar::BT_Maximize:
        if (windowState() == Qt::WindowMaximized) {
            showNormal();
        } else {
            showMaximized();
        }
        break;
    case MainTitleBar::BT_Close:
        qApp->quit();
        break;
    default:
        break;
    }
}

void MainWindow::onMainTitleBarActionTriggered(int actionType)
{
    switch (actionType) {
    case MainTitleBar::AT_AppSettings:
        d->showAppSettingsWidget();
        break;
    default:
        break;
    }
}

void MainWindow::onPictureToolBarClicked(int buttonType)
{
    switch (buttonType) {
    case PictureToolBar::BT_OpenFile: {
        if (d->pictureScene_->openFile()) {
            QTimer::singleShot(10, [&](){
                d->titleBar_->update();
            });
        }
    }
        break;
    case PictureToolBar::BT_Adaptive:
        d->pictureScene_->adaptiveToggle();
        break;
    case PictureToolBar::BT_ZoomIn:
        d->pictureScene_->zoomIn();
        break;
    case PictureToolBar::BT_ZoomOut:
        d->pictureScene_->zoomOut();
        break;
    case PictureToolBar::BT_ToPrevious:
        d->pictureScene_->toPrevious();
        break;
    case PictureToolBar::BT_ToNext:
        d->pictureScene_->toNext();
        break;
    case PictureToolBar::BT_RotateLeft:
        d->pictureScene_->rotateLeft();
        break;
    case PictureToolBar::BT_RotateRight:
        d->pictureScene_->rotateRight();
        break;
    case PictureToolBar::BT_Edit:
        break;
    default:
        break;
    }
}

void MainWindow::onThemeChanged(int type)
{
    Q_UNUSED(type);
    setBackgroundColor(G_UISETTIGNS->themeColor());
}

void MainWindow::onWindowStateChanged(Qt::WindowStates state)
{
    d->titleBar_->onWindowStateChanged(state);

    if (Qt::WindowFullScreen == state) {
        d->titleBar_->hide();
        d->pictureToolBar_->hide();
        if (!d->exitFullScreenBtn_) {
            d->exitFullScreenBtn_ = new PubPushButton(this);
            int sw = 28;
            d->exitFullScreenBtn_->setFixedSize(sw, sw);
            d->exitFullScreenBtn_->move(this->width()-sw, 0);
            d->exitFullScreenBtn_->setBackground(QLatin1String(":/images/hy_main_fullscreen_02.png"));
            QObject::connect(d->exitFullScreenBtn_, &PubPushButton::clicked, this, &MainWindow::showNormal);
        }
        d->exitFullScreenBtn_->show();
        d->exitFullScreenBtn_->raise();
    } else {
        d->titleBar_->show();
        d->pictureToolBar_->show();
        if (d->exitFullScreenBtn_) {
            d->exitFullScreenBtn_->hide();
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        if (windowState() == Qt::WindowFullScreen) {
            showNormal();
        }
        break;
    default:
        break;
    }

    PubWindow::keyPressEvent(e);
}

void MainWindow::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::ActivationChange) {
        if (this->isActiveWindow()) {
            if (d->appSettingsWidget_) {
                d->appSettingsWidget_->raise();
            }
        }
    }

    PubWindow::changeEvent(e);
}

void MainWindow::PrivateData::init()
{
    q->setMinimumSize(658, 512);
    q->resize(1090, 686);

    titleBar_ = new MainTitleBar(q);
    pictureScene_ = new PictureScene(q);
    pictureToolBar_ = new PictureToolBar(q);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar_);
    mainLayout->addWidget(pictureScene_);
    mainLayout->addWidget(pictureToolBar_);
    q->contentWidget()->setLayout(mainLayout);

    G_UISETTIGNS->mainWindow = q;
    G_UISETTIGNS->configure(QLatin1String("setup.ini"));

    QObject::connect(titleBar_, &MainTitleBar::clicked, q, &MainWindow::onMainTitleBarClicked);
    QObject::connect(titleBar_, &MainTitleBar::actionTriggered, q, &MainWindow::onMainTitleBarActionTriggered);
    QObject::connect(pictureScene_, &PictureScene::fileInfoChanged, titleBar_, &MainTitleBar::setTitle);
    QObject::connect(pictureScene_, &PictureScene::adaptiveTypeChanged, pictureToolBar_, &PictureToolBar::setAdaptiveType);
    QObject::connect(pictureToolBar_, &PictureToolBar::clicked, q, &MainWindow::onPictureToolBarClicked);
    QObject::connect(G_UISETTIGNS, &UiGlobalSettings::themeChanged, q, &MainWindow::onThemeChanged);
    QObject::connect(q, &MainWindow::windowStateChaned, q, &MainWindow::onWindowStateChanged);
}

void MainWindow::PrivateData::setWindowOverlayVisible(bool visible)
{
    if (visible) {
        if (!windowOverlay_) {
            windowOverlay_ = new QWidget(q);
            windowOverlay_->setGeometry(q->rect());
        }
        windowOverlay_->show();
        windowOverlay_->raise();
    } else {
        if (windowOverlay_) {
            delete windowOverlay_;
            windowOverlay_ = nullptr;
        }
    }
}

void MainWindow::PrivateData::showAppSettingsWidget()
{
    setWindowOverlayVisible(true);

    if (!appSettingsWidget_) {
        appSettingsWidget_ = new AppSettinsWidget;
        QObject::connect(appSettingsWidget_, &AppSettinsWidget::destroyed, [&]() {
            setWindowOverlayVisible(false);
            q->setEnabled(true);
        });
    }
    int cx = q->x() + (q->width() - appSettingsWidget_->width())/2;
    int cy = q->y() + (q->height() - appSettingsWidget_->height())/2;
    appSettingsWidget_->move(cx, cy);
    appSettingsWidget_->show();
    q->setEnabled(false);
}
