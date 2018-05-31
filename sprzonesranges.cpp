#include "sprzonesranges.h"

QObject *SPRZonesRanges::getLastCurrent() const
{
    return lastCurrent;
}

SPRSpectrumZonesModel *SPRZonesRanges::getZoneModel() const
{
    return zoneModel;
}

SPRZonesRanges::SPRZonesRanges(QWidget *parent):
    ISPRWidget(),
    zoneModel(nullptr),
    firstCollumnVisible(true),
    hHeaderVisible(true),
    selectEnable(true)
{
    verticalHeader()->setVisible(false);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

//    installEventFilter(this);
    setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
//    setEditTriggers(EditTrigger::CurrentChanged);

//    connect(this, SIGNAL(), this, SLOT(onFocusCurrentCell(QModelIndex)));
}

//void SPRZonesRanges::onFocusCurrentCell(QModelIndex index){
//        lastCurrent = cellWidget(index.row(), index.column());
//}

EnumElements SPRZonesRanges::getCurrentElement(bool *result){
    if(result) *result = false;
    EnumElements res = Ns;
    if(zoneModel){
        if(lastCurrent){
            QObject *le = lastCurrent;
            res = le->property("element").value<EnumElements>();
            if(result){
                *result = true;
            }
            return res;
        }
        QMessageBox::warning(this, QString(tr("Ошибка...")), QString(tr("Не выберан текущий элемент")));
        return Ns;
    }
    QMessageBox::warning(this, QString(tr("Ошибка...")), QString(tr("Ошибка установки модели")));
    return Ns;
}

void SPRZonesRanges::onChangeSelectedRow(bool value, int row){
    if(value){
        for(int r=0; r<rowCount(); r++){
            if(r != row){
                ((FirstCollumn2*)cellWidget(r, 0))->setSelect(false);
            }
        }
    }
}

void SPRZonesRanges::widgetsShow()
{

//    qDebug() << "SPRZonesRanges::widgetsShow(): name: "<< this->objectName();

    if(zoneModel){
//        while(rowCount() > 0){
//            removeRow(0);
//        }
        clear();
        setRowCount(zoneModel->getElementsProperty()->getElements().size());

        lastCurrent = nullptr;

        QStringList hHeaders;
        if(firstCollumnVisible){
            hHeaders << "*";
        }
        hHeaders << "<<" << ">>";

        //        int colCount = 0;
        if(firstCollumnVisible){
            setColumnCount(3);
        } else {
            setColumnCount(2);
        }

        if(hHeaderVisible){
            setHorizontalHeaderLabels(hHeaders);
//            horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        }


//        setRowCount(zoneModel->elements.size());
        QSize mySize = size();

        int row = 0;
        foreach(EnumElements el, zoneModel->getElementsProperty()->getElements().keys()){
            int colCount = 0;
            if(firstCollumnVisible){
                FirstCollumn2 *fc = new FirstCollumn2(this);
                fc->setParams(row, zoneModel->getElementsProperty()->getSName(el), zoneModel->getElementsProperty()->getColor(el), false);
                fc->setProperty("element", static_cast<QVariant>(el));
                fc->setSelectVisible(selectEnable);
//                fc->setRow(row);
//                fc->setSelectVisible(false);
//                fc->setText(model->elementsProperty->getSName(el));
//                fc->setColor(model->elementsProperty->getColor(el));
                setCellWidget(row, colCount, fc);
                fc->widgetsShow();
                QSize t1 = fc->geometry().size();
                QSize t2 = fc->sizeHint();
                fc->setMinimumSize(fc->geometry().size());
                connect(fc, SIGNAL(colorChanged(QColor)), this,SLOT(onChangeColor(QColor)));
                connect(fc, SIGNAL(deleteRow()), this, SLOT(onDeleteElements()));
                connect(fc,SIGNAL(selectedRow(bool,int)), this, SLOT(onChangeSelectedRow(bool, int)));
                //                setColumnWidth(colCount, fc->sizeHint().width());
                setColumnWidth(colCount, t1.width());
                colCount++;
            }

            QLineEdit *le = setNumberCell(this, row, colCount, zoneModel->getZones()[el]->min->getData(), 0, 256, QString(tr("Минимальное значение для ") + zoneModel->getElementsProperty()->getFName(el)));
            le->setProperty("element", static_cast<QVariant>(el));
            le->setProperty("variable", QVariant("min"));
            le->setMinimumWidth(fontMetrics().width("9999999"));
            le->installEventFilter(this);
//            le->setBaseSize(le->minimumWidth(), le->baseSize().height());
            le->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
            horizontalHeader()->resizeSection(colCount, le->size().width() + 3 *lineWidth());
            connect(le, SIGNAL(editingFinished()), this, SLOT(onChangeValue()));
            colCount++;

            le = setNumberCell(this, row, colCount, zoneModel->getZones()[el]->max->getData(), 0, 256, QString(tr("Максимальное значение для ") + zoneModel->getElementsProperty()->getFName(el)));
            le->setProperty("element", static_cast<QVariant>(el));
            le->setProperty("variable", QVariant("max"));
            QSize w = le->size();
            QSize w1 = le->sizeHint();
            le->setMinimumWidth(fontMetrics().width("9999999"));
            le->installEventFilter(this);
//            le->setBaseSize(le->minimumWidth(), le->baseSize().height());
            le->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
            w = le->size();
            w1 = le->sizeHint();
            horizontalHeader()->resizeSection(colCount, le->size().width() + 3 *lineWidth());
            connect(le, SIGNAL(editingFinished()), this, SLOT(onChangeValue()));
            row++;
        }
        this->show();
        setMinimumSize(size());
    }
}

void SPRZonesRanges::onChangeColor(QColor color){
    EnumElements el = sender()->property("element").value<EnumElements>();

    zoneModel->getElementsProperty()->setColor(el, color);
    widgetsShow();
}

void SPRZonesRanges::onDeleteElements(){
    EnumElements el = sender()->property("element").value<EnumElements>();

    zoneModel->deleteElement(el);
    emit deleteElement(zoneModel->tIndex, el);
    widgetsShow();
}


QSize SPRZonesRanges::sizeHint() const
{
//    return QTableWidget::sizeHint();
    if(zoneModel){
        int h = QTableWidget::sizeHint().height();
//        for(int row=0; row<model->elements.size();row++){
//            h += rowHeight(row) + 5* lineWidth();
//        }
        int w = 0;
        for(int col=0; col<columnCount(); col++){
            int w_max = 0;
            for(int row=0; row<rowCount();row++){
                int ww = cellWidget(row, col)->minimumWidth();
                if(w_max < ww){
                    w_max = ww;
                }
            }
            w+=w_max+2*lineWidth();
//           w += columnWidth(col)+lineWidth();
        }
        QSize res = QSize(w+lineWidth(), h);
        return res;
    } else {
        return QTableWidget::sizeHint();
    }
}

void SPRZonesRanges::resizeEvent(QResizeEvent *event)
{
    QTableWidget::resizeEvent(event);

    int newSizeWidth = event->size().width();
    int colCount = 0; int colResizeNum = columnCount();

    if(rowCount() > 0){
        if(firstCollumnVisible){
            newSizeWidth -= columnWidth(0);
            colCount++; colResizeNum--;
        }
        for(int col=colCount; col<columnCount(); col++){
            int w = columnWidth(col);
            horizontalHeader()->resizeSection(col, newSizeWidth / colResizeNum);
        }
    }
    horizontalHeader()->resizeSections(QHeaderView::Fixed);
}

void SPRZonesRanges::onChangeValue()
{
    EnumElements el = sender()->property("element").value<EnumElements>();
    QString var = sender()->property("variable").value<QString>();
    bool needWidgetShow = true;

    int value = ((QLineEdit*)sender())->text().toInt();
    if(var.toUpper() == "MIN"){
        if(zoneModel->getZones()[el]->min->getData() != value){
            needWidgetShow = true;
            zoneModel->getZones()[el]->min->setData(value);
        }
    } else if (var.toUpper() == "MAX"){
        if(zoneModel->getZones()[el]->max->getData() != value){
            needWidgetShow = true;
            zoneModel->getZones()[el]->max->setData(value);
        }
    }
    if(needWidgetShow){
        widgetsShow();
    }
}



bool SPRZonesRanges::eventFilter(QObject *watched, QEvent *event)
{

    if(event->type() == QEvent::FocusIn){
        int row = watched->property("row").value<int>(); int col = watched->property("col").value<int>();
        QModelIndex index = model()->index(row, col);
//        setCurrentCell(row, col, QItemSelectionModel::SelectCurrent);
        QLineEdit *le = (QLineEdit*)watched;
        QPalette pal = le->palette();
        pal.setColor(le->backgroundRole(), QColor(Qt::lightGray));
        le->setPalette(pal);
        lastCurrent = watched;

        if(lastCurrent)
        qDebug()<<"SPRZonesRanges->lastCurrent: name "<< lastCurrent->objectName() << "event " << event->type() << " (" <<event->registerEventType(event->type())<<")";

        emit gotFocus(this, (QLineEdit*)watched);
        return false;
    }
    if(event->type() == QEvent::FocusOut){
//        int row = watched->property("row").value<int>(); int col = watched->property("col").value<int>();
//        lastCurrent = nullptr;
        emit gotFocus(this, nullptr);
        return false;

    }
    return false;
}

//void SPRZonesRanges::focusInEvent(QFocusEvent *event)
//{
//    qDebug()<<"focusInEvent: name "<< event << "event got: " << event->gotFocus()<< "elent lost: "<< event->lostFocus()<< " (" <<event->registerEventType(event->type())<<")";
//}


void SPRZonesRanges::onModelChanget(IModelVariable *)
{
    widgetsShow();
}
