#include "sprrentgenvatable.h"
#include <QHeaderView>

SPRRentgenVATable::SPRRentgenVATable(QWidget *parent): QTableWidget(parent), model(nullptr)
{

}


void SPRRentgenVATable::widgetsShow()
{
    if(model){
        blockSignals(true);
//        while(rowCount() > 0){
//            for(int col=0; col<model->getRentgens()->getData(); col++){
//                QLineEdit* le = (QLineEdit*)cellWidget(0, col);
//                if(le){
//                    disconnect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
//                    delete le;
//                }
//            }
//            removeRow(0);
//        }

        clear();
    //    qDebug() << "setRentrenVATable ren:" + model->rentgens->toString();
        QStringList nameV; nameV << tr("I(mkA)") << tr("U(kV)");
        setColumnCount(model->getRentgens()->getData());
        setRowCount(2);
        setVerticalHeaderLabels(nameV);
        QStringList nameH;
        for(qint8 i=0, k=1; i<model->getRentgens()->getData(); i++, k++){
            nameH.append(QString::number(k));
    //        ui.tTubeVA->verticalHeader()->setSectionResizeMode(i, QHeaderView::ResizeMode::Stretch);
            QString tt = QString(tr("Ток(mkA) для трубы %1")).arg(QString::number(k));
            QLineEdit *le = setNumberCell(this, 0, i, model->iTubes[i]->getData(), 0, 1000, tt);
//            le->setProperty("table", "va");
                connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
            tt = QString(tr("Напряжение(kV) на трубе %1")).arg(QString::number(k));
            le = setNumberCell(this, 1, i, model->uTubes[i]->getData(), 0, 1000, tt);
//            le->setProperty("table", "va");
                connect(le, SIGNAL(editingFinished()), this, SLOT(viewChange()));
        }
        setHorizontalHeaderLabels(nameH);
        horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
        verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
        blockSignals(false);

    }
}

void SPRRentgenVATable::viewChange()
{
    QTableWidget *tw = sender()->property("tw").value<QTableWidget*>();
    int row = sender()->property("row").toInt();
    int col = sender()->property("col").toInt();
    QLineEdit *le = (QLineEdit*)sender();
    if(tw == this){
        uint value = le->text().toInt();
        if(row == 0){ // изиенили ток на трубе
           model->iTubes[col]->setData(value);
        } else if(row == 1){ // изиенили напряжение на трубе
           model->uTubes[col]->setData(value);
        }
        return;

    }
}

ISPRModelData *SPRRentgenVATable::setModelData(ISPRModelData *data)
{
    if(data){
        if(model){
            delete model;
        }
        model = (SPRSettingsRentgenModel*)data;
    }
    onModelChanget(model);
    return model;
}

ISPRModelData *SPRRentgenVATable::getModelData()
{
    return model;
}

void SPRRentgenVATable::onModelChanget(IModelVariable *)
{
    widgetsShow();
}
