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
#include "firstcolumn.h"

class SPRSpectrumListTable : public QTableWidget, public ISPRWidget
{
    Q_OBJECT


protected:
    SPRSpectrumListItemsModel* model;

    QList<int> storeCheckedRows;
    int storeCurrentRow;

    QLineEdit *setCellMyWidget(QTableWidget *table, int row, int col, QString value, bool editable=false, QString tt="");

    virtual void insertFirstColumn(SpectrumItemData *data, int row);
    virtual void addRowTable(SpectrumItemData *data, int pastRow = -1);
    virtual void connectFirstTable(FirstColumn *fc);
    virtual void insertContentColumns(SpectrumItemData *data, int row);
public:
    explicit SPRSpectrumListTable(QWidget *parent = 0);

    ISPRModelData *setModel(SPRSpectrumListItemsModel *_model, uint8_t *inp = nullptr);
    // ISPRWidget interface
    SPRSpectrumListItemsModel *getModels(){ return model; }
    SPRSpectrumItemModel *getModel(int index){
        if(index < model->getSpectrumsModel()->size()){
            return model->getSpectrumsModel()->at(index);
        } else {
            return nullptr;
        }
    }
    ISPRModelData *addSpectrum(uint8_t *_inp, int _bufSize = DEF_SPECTRUM_DATA_BUF_LENGTH, int _thread = -1);
//    ISPRModelData *setZonesTableModel(SPRSpectrumZonesTableModel *ranges);

    QList<int> getSelectedItems(){
        QList<int> res;
        for(int row=0; row<rowCount(); row++){
            FirstColumn *fc = (FirstColumn*)cellWidget(row, 0);
            if(fc->isRowChecked()){
                res.push_back(row);
            }
        }
        return res;
    }

    bool isSelectedItem(int row){
        FirstColumn *fc = (FirstColumn*)cellWidget(row, 0);
        return fc->isRowChecked();
    }
    QColor getColorSpectrum(int row){

        SPRSpectrumItemModel *mod = model->getSpectrumItem(row);
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
public slots:
    virtual void widgetsShow();
    virtual ISPRModelData *getModel();
    virtual void viewChange(QColor color);
    virtual void viewChange();
    virtual void viewChange(int num);
    void hideCol(bool);
    void showCols(bool);
    void onCurrentPosChanged(int row, int col);
    void onDeleteRow(int row);
signals:
    void doShow();
    void rowSelectedChecked(QList<int>, int);
    void rowChangeColor(int num);
    void rowDeleted(int row);
    void modelChanged();
};

#endif // SPRSPECTRUMLISTTABLE_H
