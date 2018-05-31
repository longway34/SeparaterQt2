#ifndef SPRELEMENTPROPERTYDIALOG_H
#define SPRELEMENTPROPERTYDIALOG_H

#include "ui_sprelementpropertydialog.h"

#include "_types.h"
#include "models/sprelementsmodel.h"
#include "models/sprspectrumzonesmodel.h"

class SPRElementPropertyDialog : public QDialog
{
    Q_OBJECT


    SPRElementsModel *elModel;
    SPRSpectrumZonesModel* spzModel;
    QList<EnumElements> elements;

public:
    Ui::SPRElementPropertyDialog ui;

    explicit SPRElementPropertyDialog(QWidget *parent = 0);
    SPRElementPropertyDialog(SPRSpectrumZonesModel *_model, EnumElements _element, QWidget *parent = 0);
    SPRElementPropertyDialog(SPRElementsModel *_model, QWidget *parent = 0);

    void Init(SPRElementsModel *_model, SPRSpectrumZonesModel *_spzModel=nullptr, QList<EnumElements> _elements=QList<EnumElements>());
public slots:
    void onElementIndexChange(int);
    void onChangeMinMax(int);
protected:
    void widgetsShow();
private:
};

#endif // SPRELEMENTPROPERTYDIALOG_H
