#ifndef SPRSEPRATEMAINTABLE_H
#define SPRSEPRATEMAINTABLE_H

#include <QObject>
#include <QTableView>
#include <QAbstractTableModel>
#include <QAbstractItemModel>

#include "models/sprmainmodel.h"
#include "models/sprspectrumlistitemsmodel.h"
#include "models/sprhistorymodel.h"
class SPRSeparateMainTableModel : public QAbstractTableModel{

    Q_OBJECT
    // QAbstractItemModel interface
    SPRThreadList threads;
    QVector<QVector<double>> hxTable;
    QVector<QVector<double>> i_prd;
    QVector<double> correlTable;

    double correlPorogPermiss, correlPorogCritical;
    EnumElements correlControlArea;

    SPRHistoryModel *historyModel;
    SPRSpectrumListItemsModel *spectrumsModel;
    SPRMainModel *mainModel;

public:
    SPRSeparateMainTableModel(SPRHistoryModel *_hist, SPRThreadList _threads, QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex &) const;
    virtual int columnCount(const QModelIndex &) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setThreads(SPRThreadList _threads);

    void setCorrelPorogs(double _permiss, double _critical, EnumElements _controlArea = Ns){
        correlPorogCritical = _critical; correlPorogPermiss = _permiss;
        correlControlArea = _controlArea;
    }

    void setHxTable(const QVector<QVector<double> > &value);
    void setHxTable();
    void setI_prd(const QVector<QVector<double> > &value);
    void setI_prd();

    void setSeparTables(const QVector<QVector<double>> _hxs, const QVector<QVector<double>> _i_prds);

    void setCorrelTable(const QVector<double> &value);
    void setCorrelTable();

    SPRHistoryModel *getHistoryModel() const;
    void setHistoryModel(SPRHistoryModel *value);

    void setSpectrumsModel(SPRSpectrumListItemsModel *value);

public slots:
    void onModelChanget(IModelVariable* send);
};

class SPRSeprateMainTable : public QTableView
{
    Q_OBJECT

    SPRSeparateMainTableModel *tableModel;
    SPRMainModel *mainModel;
    SPRHistoryModel *separateModel;
    SPRSpectrumListItemsModel *spectrumModel;

public:
    SPRSeprateMainTable(QWidget *parent = nullptr);

    virtual ~SPRSeprateMainTable();
    void init(SPRMainModel *_mainModel, SPRHistoryModel *_histModel, SPRSpectrumListItemsModel *_kSpectModel);
public slots:
    void onModelChanget(IModelVariable* source);
};

#endif // SPRSEPRATEMAINTABLE_H
