#ifndef SPRSEPARATEDETAILSTABLE_H
#define SPRSEPARATEDETAILSTABLE_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QAbstractTableModel>
#include <QAbstractItemModel>

#include "models/sprseparatemodel.h"

#include "isprwidget.h"

/**
 * @brief The SPRSeparateDetailModel class
 */
class SPRSeparateDetailModel : public QAbstractTableModel{
    Q_OBJECT

    SPRSeparateModel *separateModel;
    QVector<SPRWorkSeparateRow*> lestValidRows;
    QList<int> visibleThreads;
    int scopeDataRowEnd;
    int minTimeScope;

public:
    SPRSeparateDetailModel(QObject *parent = nullptr);
    virtual QVariant data(const QModelIndex &index, int role) const;

    ISPRModelData *setModelData(SPRSeparateModel *_model);
    SPRSeparateModel *getModelData();
    QVector<SPRWorkSeparateRow*> getValidRows(bool now=false);

    void resetNow();
    void clear();
    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    int getScopeData() const;
    void beginEndScopeData(bool value);
    void setScopeData(int value);
    QList<int> getVisibleThreads() const;
    void setVisibleThreads(const QList<int> &value);
    
    int getMinTimeScope() const;
    void setMinTimeScope(int value);

    // QAbstractItemModel interface
public:
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
public slots:
    void onModelChanget(IModelVariable *);
};

/**
 * @brief The SPRSeparateDetailsTable class
 */
class SPRSeparateDetailsTable : public QTableView, public ISPRWidget
{
    Q_OBJECT
    SPRSeparateDetailModel *myModel;
public:
    SPRSeparateDetailsTable(QWidget *parent);

    // ISPRWidget interface
public:
    virtual ISPRModelData *setModelData(ISPRModelData *data);
    virtual ISPRModelData *getModelData();
    void clear();
    SPRSeparateDetailModel *getMyModel() const;

public slots:
    virtual void onModelChanget(IModelVariable *);
    virtual void widgetsShow();
    void beginEndScope(bool value);

protected:
};

#endif // SPRSEPARATEDETAILSTABLE_H
