#include "sprdeucodestable.h"
#include <QHeaderView>


SPRDEUCodesTable::SPRDEUCodesTable(QWidget *parent):
    QTableWidget(parent), model(nullptr)
{

}

ISPRModelData *SPRDEUCodesTable::setModelData(SPRSettingsRentgenModel *data)
{
    if(data){
        if(model){
            disconnect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        }
        connect(model, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        model = data;
    }
}

void SPRDEUCodesTable::widgetsShow()
{
    blockSignals(true);
    while(rowCount() > 0){
        for(int col = 0; col < columnCount(); col++){
            QLineEdit *w = (QLineEdit*)cellWidget(0, col);
            if(w){
                disconnect(w, SIGNAL(editingFinished()), this, SLOT(viewChange()));
                delete w;
            }
        }
        removeRow(0);
    }

    setColumnCount(model->getThreads()->getData());
    setRowCount(2);
    QStringList nameV; nameV << tr("ДЭУ") << tr("ЦП");
    setVerticalHeaderLabels(nameV);
    QStringList nameH;
    for(qint8 i=0, k=1; i<model->getThreads()->getData(); i++, k++){
        QString tt = QString(tr("Код ДЭУ для ручья %1")).arg(QString::number(k));
//        ui.tDEUCode->verticalHeader()->setSectionResizeMode(i, QHeaderView::ResizeMode::Stretch);
        nameH.append(QString::number(k));
        QLineEdit *le = setNumberCell(this,0,i,model->deuCodes[i]->getData(),0,1000, tt);
        connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));

        tt = QString(tr("Код ЦП для ручья %1")).arg(QString::number(k));
        le = setNumberCell(this,1,i,model->cpCodes[i]->getData(),0,1000, tt);
        connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    }
    setHorizontalHeaderLabels(nameH);

    this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

    blockSignals(false);

}

ISPRModelData *SPRDEUCodesTable::getModelData()
{
    return model;
}

void SPRDEUCodesTable::viewChange()
{
    QObject *tw = sender()->property("tw").value<QObject*>();
    int row = sender()->property("row").toInt();
    int col = sender()->property("col").toInt();

    QLineEdit *le = (QLineEdit*)sender();

    if(tw == this){
        if(row == 0){
            model->deuCodes[col]->setData(le->text().toInt());
        } else if(row == 1){
            model->cpCodes[col]->setData(le->text().toInt());
        }
        return;
    }
}


void SPRDEUCodesTable::onModelChanget(IModelVariable *)
{
    widgetsShow();
}
