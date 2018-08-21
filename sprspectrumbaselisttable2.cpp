#include "sprspectrumbaselisttable2.h"

SPRSpectrumBaseListTable2::SPRSpectrumBaseListTable2(QWidget *parent): SPRSpectrumListTable()
{
//    hideColumn(0); // fc
//    hideColumn(1); // thread
    hideColumn(2); // name
//    hideColumn(3); // summ (I)
    hideColumn(4); // H1
    hideColumn(5); // H2
    hideColumn(6); // H3
//    hideColumn(7); // center
    hideColumn(8); // peak
//    hideColumn(9); // RS
    hideColumn(10); // time
    hideColumn(11); // date
    hideColumn(12); // Ns
    hideColumn(13); // Fe
    hideColumn(14); // Cu
    hideColumn(15); // Mo
    hideColumn(16); // Zn
    hideColumn(17); // Mg


    hHeaders << tr("№") << tr("Ручей") << tr("Имя") << tr("I") << tr("H1") << tr("H2") << tr("H3")
             << tr("Центр") << tr("Пик") << tr("X-Ray") << tr("Время") << tr("Дата и время")
             << tr("Ns") << tr("Fe") << tr("Cu") << tr("Mo") << tr("Zn") << tr("Mg");


    setHorizontalHeaderLabels(hHeaders);
}

void SPRSpectrumBaseListTable2::widgetsShow()
{
    SPRSpectrumListTable::widgetsShow();

    blockSignals(true);
    QList<SPRSpectrumItemModel*> *lst = model->getSpectrumsModel(spectrumBase);
    for(int row=0; row < lst->size(); row++){
        QLabel *la = (QLabel*)cellWidget(row, 9);
        if(la){
            double xray = lst->at(row)->getXRay();
            QString txt = QString::number(xray, 'f', 2);
            la->setText(txt);
        }
    }
    setHorizontalHeaderLabels(hHeaders);
    blockSignals(false);
}
