#ifndef UIGLOBAL_H
#define UIGLOBAL_H

#include <QObject>

class QWidget;
class MainWindow;

class UiGlobalSettings : public QObject
{
    Q_OBJECT
public:
    enum ThemeType {
        TT_CoolBlack = 0x00,
        TT_ClassicWhite = 0x01
    };

    inline static UiGlobalSettings *obj() {
        static UiGlobalSettings ins;
        return &ins;
    }

    MainWindow *mainWindow() const;
    void setMainWindowObject(MainWindow *w);

    void configure(const QString &fileName);

    int themeType() const;
    void setTheme(ThemeType type);  
    QColor themeColor();

    QString appName() const;

    void setWidgetBackgroundColor(QWidget *w, const QColor &c);

signals:
    void themeChanged(int type);

private:
    explicit UiGlobalSettings(QObject *parent = nullptr);
    ~UiGlobalSettings();
    class PrivateData;
    PrivateData *d;
};

#endif // UIGLOBAL_H
