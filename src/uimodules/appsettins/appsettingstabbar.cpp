#include "appsettingstabbar.h"
#include "pub_pushbutton.h"
#include "uiglobal.h"

#include <QVBoxLayout>
#include <QFrame>

class ToolButton : public QWidget
{
public:
    explicit ToolButton(QWidget *parent = nullptr) : QWidget(parent) {
        button_ = new PubPushButton(this);
        button_->setTextColor(QColor(0, 0, 0), QColor(88,160,229), QColor(34, 133, 234));
        button_->setCheckable(true);

        checkFrame_ = new QFrame(this);
        checkFrame_->setFixedSize(4, 22);
    }

    void setText(const QString &text) {
        button_->setText(text);
    }

    void setChecked(bool checked) {
        if (checked) {
            UiGlobalSettings::obj()->setWidgetBackgroundColor(checkFrame_, QColor(34, 133, 234));
        } else {
            UiGlobalSettings::obj()->setWidgetBackgroundColor(checkFrame_, Qt::transparent);
        }
        button_->setChecked(checked);
    }

    PubPushButton *button_;
    QFrame *checkFrame_;

protected:
    void resizeEvent(QResizeEvent *) {
        button_->setGeometry(0, 0, width() - checkFrame_->width()-10, height());
        checkFrame_->move(width() - checkFrame_->width(), (height() - checkFrame_->height())/2);
    }
};

class AppSettingsTabBar::PrivateData
{
public:
    explicit PrivateData(AppSettingsTabBar *parent) :
        q(parent)
    {}

    AppSettingsTabBar *q;
    QVector<ToolButton*> toolButtons_;

    void init();
};

AppSettingsTabBar::AppSettingsTabBar(QWidget *parent) : QWidget(parent),
    d(new PrivateData(this))
{
    d->init();
}

AppSettingsTabBar::~AppSettingsTabBar()
{
    delete d;
}

void AppSettingsTabBar::onButtonClicked()
{
    PubPushButton *btn = qobject_cast<PubPushButton*>(sender());
    int index = -1;
    for (int i = 0, j = d->toolButtons_.size(); i < j; ++i) {
        bool isEqual = d->toolButtons_.at(i)->button_ == btn;
        d->toolButtons_[i]->setChecked(isEqual);
        if (isEqual) {
            index = i;
        }
    }

    emit currentIndexChanged(index);
}

void AppSettingsTabBar::PrivateData::init()
{
    QStringList buttonNameList;
    buttonNameList << "Basic settings" << "Basic settings2";

    QVBoxLayout *mainLayout = new QVBoxLayout;

    for (auto i : buttonNameList) {
        ToolButton *button = new ToolButton(q);
        button->setFixedHeight(50);
        button->setText(i);
        toolButtons_.push_back(button);
        mainLayout->addWidget(button);
        QObject::connect(button->button_, &PubPushButton::clicked, q, &AppSettingsTabBar::onButtonClicked);
    }
    toolButtons_.at(0)->setChecked(true);

    mainLayout->addStretch();
    mainLayout->setContentsMargins(0, 10, 0, 10);
    mainLayout->setSpacing(0);
    q->setLayout(mainLayout);
}
