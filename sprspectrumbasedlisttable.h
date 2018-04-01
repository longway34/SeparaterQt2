#ifndef SPRSPECTRUMBASEDLISTTABLE_H
#define SPRSPECTRUMBASEDLISTTABLE_H

#include <QObject>
#include <QWidget>

#include "sprspectrumlisttable.h"

class SPRSpectrumBasedListTable : public SPRSpectrumListTable
{
public:
    SPRSpectrumBasedListTable(QWidget *parent = nullptr);

    // ISPRWidget interface
public:
    virtual void widgetsShow();

    // SPRSpectrumListTable interface
protected:
    virtual void insertFirstColumn(SpectrumItemData *data, int row);
    virtual void connectFirstTable(FirstColumn *fc);
    virtual void insertContentColumns(SpectrumItemData *data, int row);

};

#endif // SPRSPECTRUMBASEDLISTTABLE_H
