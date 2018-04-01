#include <QMessageBox>
#include <QWidget>

#include "sprspectrumzonestablewidget.h"
#include "sprspectrumranges.h"

SPRSpectrumZonesTableWidget::SPRSpectrumZonesTableWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    QFont font = parentWidget()->font();
    setFont(font);

    connect(this, SIGNAL(doShow()), ui.wSpectrumTable, SLOT(widgetsShow()));
    connect(ui.bEquialent, SIGNAL(clicked(bool)), this, SLOT(viewChange(bool)));
    connect(ui.wSpectrumTable, SIGNAL(changeColor(EnumElements,QColor)), this, SLOT(onChangeColor(EnumElements,QColor)));
}


void SPRSpectrumZonesTableWidget::widgetsShow()
{
    ui.wSpectrumTable->widgetsShow();
}

ISPRModelData *SPRSpectrumZonesTableWidget::getModel()
{
    return ui.wSpectrumTable->getModel();
}

SPRSpectrumZonesTableModel *SPRSpectrumZonesTableWidget::setModel(SPRSpectrumZonesTableModel *data)
{
    ui.wSpectrumTable->setModel(data);
    return data;
}

void SPRSpectrumZonesTableWidget::viewChange(QTableWidget *, int, int)
{
    ui.bEquialent->setEnabled(true);
}

void SPRSpectrumZonesTableWidget::viewChange(bool)
{
    QLineEdit *focusedLe = (QLineEdit*)ui.wSpectrumTable->focusWidget();
    if(!focusedLe){
        QMessageBox msg(QMessageBox::Icon::Critical, tr("Нет выбранного элемента..."), tr("Нет выбранного элемента..."), QMessageBox::Ok);
        msg.exec();
        return;
    }
    if(!focusedLe->objectName().startsWith("leTable")){
        QMessageBox msg(QMessageBox::Icon::Critical, tr("Нет выбранного элемента..."), tr("Нет выбранного элемента..."), QMessageBox::Ok);
        msg.exec();
        return;
    }

    int column = QVariant(focusedLe->property("col")).toInt();
    uint val = focusedLe->text().toUInt();
    EnumElements el = static_cast<EnumElements>(QVariant(focusedLe->property("element")).toInt());
    SPRSpectrumZonesTableModel *mod = (SPRSpectrumZonesTableModel*)ui.wSpectrumTable->getModel();
    uint th = mod->getThreads()->getData();

    for(int i=0; i<th; i++){
        if(column == 0){
            ((SPRSpectrumZonesModel*)mod->items[i])->elements[el].min->setData(val);
        } else {
            ((SPRSpectrumZonesModel*)mod->items[i])->elements[el].max->setData(val);
        }
    }
    ui.wSpectrumTable->widgetsShow();
}
