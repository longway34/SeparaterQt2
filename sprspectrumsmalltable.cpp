#include "sprspectrumsmalltable.h"

SPRSpectrumSmallTable::SPRSpectrumSmallTable(QWidget *parent): SPRSpectrumListTable(parent)
{
//    hideColumn(0);
    hideColumn(2); /*hideColumn(3);*/
    /*hideColumn(7);*/ hideColumn(8); //hideColumn(9);
    hideColumn(11); hideColumn(12); hideColumn(13); hideColumn(14); hideColumn(15); hideColumn(16); hideColumn(17);

    verticalHeader()->setVisible(true);
}

void SPRSpectrumSmallTable::widgetsShow(){
    SPRSpectrumListTable::widgetsShow();
    for(int row=0; row<spectrums->size(); row++){
        SPRSpectrumItemModel *mod = spectrums->at(row);
        ((QLabel*)cellWidget(row, 10))->setText(QString::number(*mod->getSpectrumData()->time / 1000, 'f', 3));
        ((QLabel*)cellWidget(row, 9))->setText(QString::number(mod->getSpectrumData()->correl, 'f', 3));
    }
    resizeColumnsToContents();
}
