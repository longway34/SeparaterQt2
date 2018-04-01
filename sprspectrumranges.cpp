//#include "algorithm"
#include <QSize>

#include <QLineEdit>
#include <QHeaderView>
#include <QFocusEvent>
#include <QFont>
#include <colorbutton.h>

#include "sprspectrumranges.h"

void SPRSpectrumRanges::setModel(SPRSpectrumZonesModel *value)
{
    model = value;
    if(model){
        this->clear();
        QStringList nameH; 
//        if(model->tIndex != 0){
//            nameH << tr("<<") << tr(">>");
//            setColumnCount(2);
//        } else {
//            nameH << "*" << tr("<<") << tr(">>");
//            setColumnCount(3);
//        }

        nameH << "*" << tr("<<") << tr(">>");
        setColumnCount(3);


        this->setHorizontalHeaderLabels(nameH);
        setRowCount(model->elements.size());
        setAlternatingRowColors(true);
//        setStyleSheet("alternate-background-color: rgb(201, 255, 198);");
    //    setRowCount(1);
        QStringList nameV;
        int count = 0;
//        actualSize = QSize(0,horizontalHeader()->height());
        foreach(EnumElements el, model->elements.keys()){
            int colCount = 0;
//            if(model->tIndex == 0){
                ColorButton *cb = new ColorButton(model->elements[el].color);
                QVariant qv_tw;
                qv_tw.setValue<QTableWidget*>(this);
                cb->setProperty("tw", qv_tw);
                cb->setProperty("row", QVariant(count));
                cb->setProperty("element", QVariant(el));
                setCellWidget(count, 0, cb);
                connect(cb, SIGNAL(changeColor(QColor)), this, SLOT(viewChange(QColor)));
                colCount++;
//            }
            
            nameV.append(DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el].sname);
            QString tt = tr("Минимальное значение канала для елемента'") +
                    QString::number(model->tIndex)+tr(" елемента '") + DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el].sname +"'";
            QLineEdit *cell = setNumberCell(this, count, colCount, model->elements[el].min->getData(), 0, MAX_SPR_SPECTOR_CHANNELS,tt);
            cell->setProperty("min_max", QVariant("min"));
            colCount++;
            cell->setProperty("element", QVariant(el));
            cell->setProperty("thread", model->tIndex);
            cell->setMaximumSize(std::max(fontMetrics().width("9999999"),fontMetrics().width(horizontalHeaderItem(0)->text())), 16777215);
            cell->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
            connect(cell, SIGNAL(editingFinished()), this, SLOT(viewChange()));
            tt = tr("Максимальное значение канала ")+
                    QString::number(model->tIndex)+tr(" елемента '") + DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el].sname +"'";
            cell = setNumberCell(this, count, colCount, model->elements[el].max->getData(), 0, MAX_SPR_SPECTOR_CHANNELS,tt);
            cell->setProperty("min_max", QVariant("max"));
            cell->setProperty("element", QVariant(el));
            cell->setProperty("thread", model->tIndex);
            cell->setMaximumSize(std::max(fontMetrics().width("9999999"),fontMetrics().width(horizontalHeaderItem(1)->text())), 16777215);
            cell->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
            connect(cell, SIGNAL(editingFinished()), this, SLOT(viewChange()));
    //        connect(cell, QWidget::focusInEvent(QFocusEvent * event), this, SLOT(onFocus(QFocusEvent*)));
//            actualSize.setHeight(actualSize.height()+rowHeight(count));
//            if(count == 0){
//                if(model->tIndex == 0){
//                    actualSize.setWidth(columnWidth(0) + columnWidth(1) * 2);
//                } else {
//                    actualSize.setWidth(columnWidth(0) * 2);
//                }
//            }
            count++;
        }
        this->setVerticalHeaderLabels(nameV);
//        if(model->tIndex == 0){
//            this->setVerticalHeaderLabels(nameV);
//            actualSize.setWidth(actualSize.width() + verticalHeader()->size().width());
//        } else {
//    //        this->setVerticalHeader(nullptr);
//            this->verticalHeader()->setVisible(false);
//        }
        resizeColumnsToContents();
        resizeRowsToContents();

        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

QPoint SPRSpectrumRanges::getSelectedPosition()
{
    if(!focusWidget()){
        return QPoint(-1, -1);
    } else {
        QLineEdit *le = (QLineEdit*)focusWidget();
        EnumElements el = static_cast<EnumElements>(QVariant(le->property("element")).toInt());
        int row = DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el].index;
        QWidget *wid = cellWidget(row, 0);
        int col;
//        if(firstStateView){
//            if(model->tIndex == 0){
            col = focusWidget() == cellWidget(row, 1) ? 1 : 2;
//        } else {
//            col = focusWidget() == cellWidget(row, 0) ? 0 : 1;
//        }
        return QPoint(row, col);
    }
}

EnumElements SPRSpectrumRanges::getSelectedElement(){
    if(!focusWidget()){
        return Ns;
    } else {
        QLineEdit *le = (QLineEdit*)focusWidget();
        EnumElements el = static_cast<EnumElements>(QVariant(le->property("element")).toInt());
        return el;
    }
}

void SPRSpectrumRanges::setFirstStateView(bool value)
{
    firstStateView = value;
    widgetsShow();
}

SPRSpectrumRanges::SPRSpectrumRanges(QWidget *parent) :
    QTableWidget(parent), model(nullptr), firstStateView(true)
{
}

void SPRSpectrumRanges::widgetsShow()
{
    if(model){
//        int minColumn = model->tIndex == 0 ? 1 : 0;
//        int maxColumn = model->tIndex == 0 ? 2 : 1;
        int minColumn = 1;
        int maxColumn = 2;
        foreach(EnumElements el, model->elements.keys()){
            int row = DEF_SPR_FORMULA_ELEMENTS_PROPERTY[el].index;
            QLineEdit *le = (QLineEdit*)cellWidget(row, minColumn);
            le->setText(model->elements[el].min->toString());
            le = (QLineEdit*)cellWidget(row, maxColumn);
            le->setText(model->elements[el].max->toString());
        }
        if(firstStateView){
            this->verticalHeader()->setVisible(true);
            this->setColumnHidden(0, false);
        } else {
            this->verticalHeader()->setVisible(false);
            this->setColumnHidden(0, true);
        }
//        resizeColumnsToContents();
    }
}

SPRSpectrumZonesModel *SPRSpectrumRanges::getModel()
{
    return model;
}

QSize SPRSpectrumRanges::sizeHint() const
{
    if(model){
        int h = this->horizontalHeader()->sizeHint().height();
        for(int i=0; i<model->elements.size(); i++){
            h += rowHeight(i);
        }
        int w = columnWidth(1) + columnWidth(2) + 3 * lineWidth();
//        if(model->tIndex == 0){
//            w += columnWidth(2)+this->verticalHeader()->sizeHint().width() + lineWidth();
//        }
        if(firstStateView){
            w += columnWidth(0)+this->verticalHeader()->sizeHint().width() + lineWidth();
        }

        QSize res(w,h);
        return res;
    } else {
        return QTableWidget::sizeHint();
    }
}


void SPRSpectrumRanges::viewChange()
{
    QTableWidget *tw = sender()->property("tw").value<QTableWidget*>();
    QLineEdit *le = (QLineEdit*)sender();
    if(tw == this){
        EnumElements element = le->property("element").value<EnumElements>();
        int row = le->property("row").toInt();
        int col = le->property("col").toInt();
//        if(model->tIndex == 0) {
//            col = col - 1;
//        }
        uint value = le->text().toInt();
        if(col == 1){ // изиенили минимум для канала
           model->elements[element].min->setData(value);
        } else if(col == 2){ // изиенили максимум для канала
           model->elements[element].max->setData(value);
        }
        emit tableChange(this, row, col);

    }
}

void SPRSpectrumRanges::viewChange(QColor color)
{
    QTableWidget *tw = sender()->property("tw").value<QTableWidget*>();
    if(tw == this){
        EnumElements el = sender()->property("element").value<EnumElements>();
        model->elements[el].color = color;
        emit changeColor(el, color);
    }
}
