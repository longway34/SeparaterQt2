#include "sprzonesrangestable.h"
#include "sprzonesranges.h"

SPRZonesRanges *SPRZonesRangesTable::getLastCurrentTable() const
{
    return lastCurrentTable;
}

QLineEdit *SPRZonesRangesTable::getLastCurrentCells() const
{
    return lastCurrentCells;
}

void SPRZonesRangesTable::resizeEvent(QResizeEvent *event)
{
    QTableWidget::resizeEvent(event);

    int newWidth = event->size().width();
    for(int col=0; col<columnCount(); col++){
        horizontalHeader()->resizeSection(col, newWidth/columnCount());
    }
}

void SPRZonesRangesTable::setSelectButtonEnable(bool value)
{
    selectButtonEnable = value;
    widgetsShow();
}

SPRZonesRangesTable::SPRZonesRangesTable(QWidget *parent):
    ISPRWidget(), ztModel(nullptr), selectButtonEnable(false)
{
    verticalHeader()->setVisible(false);
//    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

//    connect(this,SIGNAL(clicked(QModelIndex)), this, SLOT(onFocusCurrentCell(QModelIndex)));
}

void SPRZonesRangesTable::setModelData(SPRSpectrumZonesTableModel *_model){
    if(_model && ztModel != _model){
        if(ztModel){
            disconnect(ztModel, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
        }
        ztModel = _model;
        connect(ztModel, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
    }
    //        widgetsShow();
}

//void SPRZonesRangesTable::onFocusCurrentCell(QModelIndex index){
//        lastCurrent = cellWidget(index.row(), index.column());
//}

EnumElements SPRZonesRangesTable::getCurrentElement(bool *result){
    if(result) *result = false;
    if(ztModel){
        if(lastCurrentTable){
            EnumElements res = lastCurrentElement;
            if(result) *result = true;
            return res;
        } else {
            QMessageBox::warning(this, QString(tr("Ошибка...")), QString(tr("Не выберан текущий элемент")));
            return Ns;
        }
    }
    QMessageBox::warning(this, QString(tr("Ошибка...")), QString(tr("Ошибка установки модели")));
    return Ns;
}

void SPRZonesRangesTable::onGotFocusEvent(SPRZonesRanges *_thread, QLineEdit *_le){

    if(_thread){
        lastCurrentTable = _thread;
        lastCurrentCells = _le;
        if(lastCurrentTable){
            if(_le){
                lastCurrentElement = _le->property("element").value<EnumElements>();
                lastCurrentMinMax = _le->property("variable").value<QString>();
                lastCurrentCells = _le;
            }

        }

    }
    qDebug()<<"onGotFocusEvent: SPRZonesRangesTable->name "<<this->objectName()<< "SPRZonesRangesTable->lastCurrent->name "<< lastCurrentTable->objectName()<< "le: "<< _le;
    emit gotFocus(lastCurrentTable, lastCurrentCells);
//    gotFocus(_thread, _le);
}

void SPRZonesRangesTable::setThreadsVisible(QList<int> _visible){
    threadVisible = _visible;
    widgetsShow();
}

void SPRZonesRangesTable::widgetsShow()
{
//    qDebug() << "SPRZonesRanges::widgetsShow(): name: "<< this->objectName();
    if(ztModel){
        this->clear();
        lastCurrentTable = nullptr;
        lastCurrentElement = Ns;
        lastCurrentMinMax = "";
        lastCurrentCells = nullptr;
        QStringList hHeaders;
        setRowCount(1);
        setColumnCount(threadVisible.size()==0 ? ztModel->items.size() : threadVisible.size());

        int heightMax = 0;
        //    horizontalHeader()->setVisible(false);

        for(int i=0; i<ztModel->items.size(); i++){
            if(threadVisible.contains(i) || threadVisible.size() == 0){
                hHeaders << QString(tr("Ручей %1")).arg(QString::number(ztModel->items[i]->tIndex+1));
            }
        }
        setHorizontalHeaderLabels(hHeaders);

        for(int i=0, count=0; i<ztModel->items.size(); i++){
            if(threadVisible.contains(i) || threadVisible.size() == 0){
                SPRZonesRanges *thZone = new SPRZonesRanges(this);
                thZone->setSelectEnable(selectButtonEnable);

                thZone->setObjectName(QString("thZone_%1").arg(QString::number(i)));

                if(count == 0){
                    thZone->setFirstCollumnVisible(true);
                } else {
                    thZone->setFirstCollumnVisible(false);
                }
                thZone->setModelData(ztModel->items[i]);
                //                thZone->widgetsShow();
                connect(thZone, SIGNAL(deleteElement(int,EnumElements)), this, SLOT(onDeleteElement(int,EnumElements)));
                connect(thZone, SIGNAL(gotFocus(SPRZonesRanges*, QLineEdit*)), this, SLOT(onGotFocusEvent(SPRZonesRanges*, QLineEdit*)));

                setCellWidget(0, count, thZone);
                thZone->widgetsShow();
                if(heightMax < thZone->sizeHint().height()){
                    heightMax = thZone->sizeHint().height();
                }
                int www = thZone->sizeHint().width();
                horizontalHeader()->resizeSection(count, www);
                count++;
            }
        }
        horizontalHeader()->resizeSections(QHeaderView::Stretch);
        resizeColumnsToContents();
        resizeRowsToContents();
//        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    }
}

void SPRZonesRangesTable::onDeleteElement(int thread, EnumElements el){
    for(int i=0; i<ztModel->getThreads()->getData(); i++){
        ztModel->items[i]->deleteElement(el);
    }
    widgetsShow();
}

SPRSpectrumZonesTableModel *SPRZonesRangesTable::getZtModel(){
    return ztModel;
}

ISPRModelData *SPRZonesRangesTable::getModelData()
{
    return ztModel;
}




void SPRZonesRangesTable::onModelChanget(IModelVariable *)
{
    widgetsShow();
}
