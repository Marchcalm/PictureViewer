#ifndef PICTUREQUICKVIEWBUTTON_H
#define PICTUREQUICKVIEWBUTTON_H

#include "pub_pushbutton.h"

class PictureQuickViewButton : public PubPushButton
{
    Q_OBJECT
public:
    explicit PictureQuickViewButton(QWidget *parent = nullptr);
    ~PictureQuickViewButton();

    void requestShow();
    void requestHide();

private:
    class PrivateData;
    PrivateData *d;
};

#endif // PICTUREQUICKVIEWBUTTON_H
