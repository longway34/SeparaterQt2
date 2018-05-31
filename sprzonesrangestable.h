#ifndef SPRZONESRANGESTABLE_H
#define SPRZONESRANGESTABLE_H

#include <QObject>
#include <QWidget>

#include <QTableWidget>
#include <QMessageBox>

#include "isprwidget.h"
#include "models/sprspectrumzonestablemodel.h"
#include "sprzonesranges.h"

class SPRZonesRangesTable : public QTableWidget, ISPRWidget
{
    Q_OBJECT

    SPRSpectrumZonesTableModel *ztModel;
    QList<int> threadVisible;

    SPRZonesRanges *lastCurrentTable;
    QLineEdit *lastCurrentCells;
    EnumElements lastCurrentElement;
    QString lastCurrentMinMax;
    bool selectButtonEnable;
public:
    SPRZonesRangesTable(QWidget *parent);

    EnumElements getCurrentElement(bool *result = nullptr);

    virtual ISPRModelData *getModelData();
    void setModelData(SPRSpectrumZonesTableModel *_model);

    SPRSpectrumZonesTableModel *getZtModel();
    SPRZonesRanges *getLastCurrentTable() const;

    int getLastCurrentThread(){
        int res = -1;
        if(lastCurrentTable){
            if(lastCurrentTable->getZoneModel()){
                res = lastCurrentTable->getZoneModel()->tIndex;
            }
        }
        return res;
    }

    QLineEdit *getLastCurrentCells() const;
    void setSelectButtonEnable(bool value);

protected:
    void resizeEvent(QResizeEvent *event);

public slots:
    void onGotFocusEvent(SPRZonesRanges *_thread, QLineEdit *_le);

    void setThreadsVisible(QList<int> _visible = QList<int>());
    void setThreadsVisible(int th){
        setThreadsVisible(QList<int>({th}));
    }
    void widgetsShow();

    void onDeleteElement(int thread, EnumElements el);
    virtual void onModelChanget(IModelVariable *);

signals:
    void gotFocus(SPRZonesRanges *_thread, QLineEdit *_le);

    // ISPRWidget interface
protected:
};







#endif // SPRZONESRANGESTABLE_H
