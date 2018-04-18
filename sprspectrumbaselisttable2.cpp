#include "sprspectrumbaselisttable2.h"

SPRSpectrumBaseListTable2::SPRSpectrumBaseListTable2(QWidget *parent): SPRSpectrumListTable()
{
//    hideColumn(0); // fc
//    hideColumn(1); // thread
//    hideColumn(2); // name
    hideColumn(3); // summ (I)
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


    QStringList hHeaders; hHeaders << tr("№") << tr("Ручей") << tr("Центр") << tr("X-Ray");

    setVerticalHeaderLabels(hHeaders);
}
