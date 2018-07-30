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

    QVector<SPRSpectrumItemModel*> *spectrums;

    QList<SPRSpectrumItemModel*> storeCheckedItems;
    SPRSpectrumItemModel* storeCurrentItem;

//    static uint keyCount;

    QLineEdit *setCellMyWidget(QTableWidget *table, int row, int col, QString value, bool editable=false, QString tt="");

    virtual void insertFirstColumn(SpectrumItemData *data, int row);
    virtual void addRowTable(SpectrumItemData *data, int pastRow = -1);
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

    SPRSpectrumItemModel *getModelData(int index){
        if(index < spectrums->size()){
            return spectrums->at(index);
        } else {
            return nullptr;
        }
    }
//    SPRSpectrumItemModel *addSpectrum(uint8_t *_inp, int _bufSize = DEF_SPECTRUM_DATA_BUF_LENGTH, int _thread = -1);
//    ISPRModelData *setZonesTableModel(SPRSpectrumZonesTableModel *ranges);


    QList<int> getSelectedItemsNumbers(){
        QList<int> res;
        for(int row=0; row<rowCount(); row++){
            FirstCollumn2 *fc = (FirstCollumn2*)cellWidget(row, 0);
            if(fc->isSelect()){
                res.push_back(row);
            }
        }
        return res;
    }

    QList<SPRSpectrumItemModel*> getSelectedItems(){
        QList<SPRSpectrumItemModel*> res;
        for(int row=0; row<rowCount();row++){
            FirstCollumn2 *fc = (FirstCollumn2*)cellWidget(row, 0);
            if(fc->isSelect()){
                SPRSpectrumItemModel* item = model->getSpectrumItem(row, typeData);
                if(item)
                    res.push_back(item);
            }

        }
        return res;
    }

    bool isSelectedItem(int row){
        FirstCollumn2 *fc = (FirstCollumn2*)cellWidget(row, 0);
        return fc->isSelect();
    }
    QColor getColorSpectrum(int row){

        SPRSpectrumItemModel *mod = spectrums->at(row);
        if(mod){
            QColor ret = mod->getSpectrumColor();
            return ret;
        }
        return QColor(Qt::black);
    }
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
