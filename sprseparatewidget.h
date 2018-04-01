#ifndef SPRSEPARATEWIDGET_H
#define SPRSEPARATEWIDGET_H

#include "ui_sprseparatewidget.h"

class SPRSeparateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SPRSeparateWidget(QWidget *parent = 0);

private:
    Ui::SPRSeparateWidget ui;
};

#endif // SPRSEPARATEWIDGET_H
