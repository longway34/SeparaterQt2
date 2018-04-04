#include "sprspectrumsmalltable.h"

SPRSpectrumSmallTable::SPRSpectrumSmallTable(QWidget *parent): SPRSpectrumListTable(parent)
{
    hideColumn(0);
    hideColumn(2); hideColumn(3);
    hideColumn(7); hideColumn(8); hideColumn(9);
    hideColumn(11); hideColumn(12); hideColumn(13); hideColumn(14); hideColumn(15); hideColumn(16); hideColumn(17);

    verticalHeader()->setVisible(true);
}
