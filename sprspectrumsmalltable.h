#ifndef SPRSPECTRUMSMALLTABLE_H
#define SPRSPECTRUMSMALLTABLE_H

#include <QObject>
#include <QWidget>

#include "sprspectrumlisttable.h"
#include "variables/sprenumvariable.h"

class SPRSpectrumSmallTable : public SPRSpectrumListTable
{
public:
    explicit SPRSpectrumSmallTable(QWidget *parent = 0);
    virtual void widgetsShow();


};

#endif // SPRSPECTRUMSMALLTABLE_H
