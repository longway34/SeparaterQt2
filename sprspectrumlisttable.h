#ifndef SPRSPECTRUMLISTTABLE_H
#define SPRSPECTRUMLISTTABLE_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QFile>
#include <QVector>
#include <QLabel>

#include "models/sprspectrumlistitemsmodel.h"
#include "isprwidget.h"
#include "firstcollumn2.h"

class SPRSpectrumListTable : public QTableWidget, public ISPRWidget
{
    Q_OBJECT


protected:
    SPRSpectrumListItemsModel* model;

    QList<SPRSpectrumItemModel*> *spectrums;

    QList<SPRSpectrumItemModel*> storeCheckedItems;
    SPRSpectrumItemModel* storeCurrentItem;

//    static uint keyCount;

    QLineEdit *setCellMyWidget(QTableWidget *table, int row, int col, QString value, bool editable=false, QString tt="");

    virtual void insertFirstColumn(SpectrumItemData *data, int row);
    virtual void setRowTable(SpectrumItemData *data, int row);
    virtual void connectFirstTable(FirstCollumn2 *fc);
    virtual void insertContentColumns(SpectrumItemData *data, int row);

    SPRTypeSpectrumSet typeData;
public:
    explicit SPRSpectrumListTable(QWidget *parent = 0);

    ISPRModelData *setModelData(SPRSpectrumListItemsModel *_model, SPRTypeSpectrumSet _type);
    virtual ISPRModelData *setModelData(ISPRModelData *_model){return setModelData((SPRSpectrumListItemsModel*)_model, spectrumsOnly);}
    // ISPRWidget interface
    SPRSpectrumListItemsModel *getModels(){ return model; }
    virtual ISPRModelData *getModelData()
    {
        return model;
    }

    SPRSpectrumItemModel *getModelData(int index);
//    SPRSpectrumItemModel *addSpectrum(uint8_t *_inp, int _bufSize = DEF_SPECTRUM_DATA_BUF_LENGTH, int _thread = -1);
//    ISPRModelData *setZonesTableModel(SPRSpectrumZonesTableModel *ranges);


    QList<int> getSelectedItemsNumbers();

    QList<SPRSpectrumItemModel*> getSelectedItems();

    bool isSelectedItem(int row){
        FirstCollumn2 *fc = (FirstCollumn2*)cellWidget(row, 0);
        return fc->isSelect();
    }
    QColor getColorSpectrum(int row);
    int getThread(int row){
        QLabel* lth = (QLabel*)cellWidget(row, 1);
        return lth->text().toInt();
    }
    int getSumm(int row){
        QLabel* lth = (QLabel*)cellWidget(row, 3);
        return lth->text().toInt();
    }
    QString getName(int row){
        QLabel* lth = (QLabel*)cellWidget(row, 2);
        return lth->text();
    }
protected:
    void resizeEvent(QResizeEvent *event);

    int getMaxColumnWidth(int col);

public slots:
    virtual void widgetsShow();
//    virtual SPRSpectrumListItemsModel *getModel();
    virtual void viewChange(QColor color);
    virtual void viewChange();
    virtual void viewChange(int num);
    void hideCol(bool);
    void showCols(bool);
    void onCurrentPosChanged(int row, int col);
    void onDeleteRow(int row);
    void onRowSelect(bool checked, int row);
    virtual void onModelChanget(IModelVariable *);
signals:
    void doShow();
    void rowSelectedChecked(QList<int>, int);
    void rowSelectedChecked3(QList<SPRSpectrumItemModel*>, SPRSpectrumItemModel*);
    void rowSelectedChecked2(int, SPRSpectrumListTable*);
    void itemChangeColor(int num);
    void itemChangeColor(SPRSpectrumItemModel*, QColor);
    void rowDeleted(int row);

    // ISPRWidget interface
public:

    // ISPRWidget interface
    SPRTypeSpectrumSet getTypeData() const;

protected:
    int findRowByItemModel(SPRSpectrumItemModel *mod);
};



#endif // SPRSPECTRUMLISTTABLE_H
