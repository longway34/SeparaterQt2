#ifndef SPRSPECTRUMCHOISETIMEDIALOG_H
#define SPRSPECTRUMCHOISETIMEDIALOG_H

#include "ui_sprspectrumchoisetimedialog.h"
#include "variables/sprvariable.h"
#include "sprsettingsrentgenwidget.h"
#include "models/sprmainmodel.h"

class SPRSpectrumChoiseTimeDialog : public QDialog
{
    Q_OBJECT

    SPRMainModel *model;
    SPRSettingsRentgenWidget *rentgenWidget;
    int threads;

public:
    explicit SPRSpectrumChoiseTimeDialog(QWidget *parent = 0);

    SPRSpectrumChoiseTimeDialog(SPRMainModel *_model, QWidget *parent = 0);
    QList<uint8_t> getThreads();
    double getTime();
    void setModelData(SPRMainModel *_model = 0);
    bool isRentgenOn();
    bool isRentgenOff();
    bool isRGUDown();
    QString getPrefix();
public slots:
    void onShowRengenParams(bool);
    void onAllThreadClick(bool value);
private:
    Ui::SPRSpectrumChoiseTimeDialog ui;
};

#endif // SPRSPECTRUMCHOISETIMEDIALOD_H
