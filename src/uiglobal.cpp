#include "uiglobal.h"

#include <QWidget>
#include <QApplication>
#include <QResource>
#include <QMap>
#include <QDebug>

class UiGlobalSettings::PrivateData
{
public:
    explicit PrivateData(UiGlobalSettings *parent) :
        q(parent), themeType_(-1)
    {}

    UiGlobalSettings *q;    
    int themeType_;
    QMap<int, QString> rccMap_;

    void init();
};

void UiGlobalSettings::configure(const QString &fileName)
{
    Q_UNUSED(fileName)

    QFont f(qApp->font());
    f.setFamily("Microsoft YaHei");
    qApp->setFont(f);

    setTheme(UiGlobalSettings::TT_CoolBlack);
}

int UiGlobalSettings::themeType() const
{
    return d->themeType_;
}

void UiGlobalSettings::setTheme(UiGlobalSettings::ThemeType type)
{
    if (d->themeType_ != type) {
        QResource::unregisterResource(d->rccMap_.value(d->themeType_));
        d->themeType_ = type;
        bool isOk = QResource::registerResource(d->rccMap_.value(type));
        if (!isOk) {
            qDebug() << "registerResource failed!";
        }
        emit themeChanged(d->themeType_);
    }
}

QColor UiGlobalSettings::themeColor()
{
    switch (d->themeType_) {
    case UiGlobalSettings::TT_CoolBlack: return QColor(42, 42, 42);
    case UiGlobalSettings::TT_ClassicWhite: return QColor(245, 245, 245);
    }

    return QColor(245, 245, 245);
}

QString UiGlobalSettings::appName() const
{
    return QLatin1String("Picture Viewer");
}

void UiGlobalSettings::setWidgetBackgroundColor(QWidget *w, const QColor &c)
{
    if (w) {
        QPalette pal(w->palette());
        pal.setColor(QPalette::Window, c);
        w->setPalette(pal);
        w->setAutoFillBackground(true);
    }
}

UiGlobalSettings::UiGlobalSettings(QObject *parent) : QObject(parent),
    d(new PrivateData(this))
{
    d->init();
}

UiGlobalSettings::~UiGlobalSettings()
{
    delete d;
}

void UiGlobalSettings::PrivateData::init()
{
    rccMap_.insert(UiGlobalSettings::TT_CoolBlack, QLatin1String("skin/coolblack.rcc"));
    rccMap_.insert(UiGlobalSettings::TT_ClassicWhite, QLatin1String("skin/classicwhite.rcc"));
}
