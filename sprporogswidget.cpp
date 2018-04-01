#include "sprporogswidget.h"
#include <QObjectUserData>

static SeparatorPorogsData defSeparatorPorogsData = {
  4, 6,
    {{1., 99., -99., 99., -99, 99},
    {1., 99., -99., 99., -99, 99},
    {1., 99., -99., 99., -99, 99},
    {1., 99., -99., 99., -99, 99},
    {1., 99., -99., 99., -99, 99},
    {1., 99., -99., 99., -99, 99}}
};

SPRPorogsWidget::SPRPorogsWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
}

void SPRPorogsWidget::setThreads(SPRVariable<uint> *threads) {
    this->model->setThreads(threads);
}

ISPRModelData *SPRPorogsWidget::setModel(ISPRModelData *model){
    this->model = (SPRPorogsModel*)model;
    return model;
}

void SPRPorogsWidget::widgetsShow()
{
    if(model){
        ui.tPorogsValues->blockSignals(true);
        ui.tPorogsValues->clear();
        SPRVariable<TypeConditions> *condv = model->getConditions();
        SPRVariable<uint> *thr = model->getThreads();
        int cond = mapConditions[model->getConditions()->getData()];
        ui.tPorogsValues->setRowCount(cond);
        ui.tPorogsValues->setColumnCount(model->getThreads()->getData());
        QVector<QString> *names = new QVector<QString>({ tr("H1 >"),
                                       tr("и H2 <"),
                                       tr("или H2 <"),
                                       tr("или H2 >"),
                                       tr("или H3 <"),
                                       tr("или H3 >")
                                      });
        QStringList vnames;
        QStringList hnames;
        for(int t=0, k=1; t < model->getThreads()->getData(); t++, k++){
            hnames.append(QString::number(k));
            for(int c=0; c<cond; c++){
                if(!t) vnames.append(names->at(c));
                QLineEdit *cellValue = new QLineEdit(QString::number(model->porogs[t][c]->getData()),ui.tPorogsValues);
                cellValue->setFrame(false); cellValue->setAlignment(Qt::AlignCenter);
                cellValue->setValidator(new QDoubleValidator(-99,99,2,cellValue));
                cellValue->setToolTip(tr("Значение для ручья ")+QString::number(t)+tr(" условия ")+names->at(c));
                cellValue->setProperty("row", c); cellValue->setProperty("col", t); //cellValue->setProperty("parent", QVariant("tPorogsValues"));
                ui.tPorogsValues->setCellWidget(c, t, cellValue);
                connect(cellValue, SIGNAL(editingFinished()), this, SLOT(viewChange()));
            }
        }
        ui.tPorogsValues->setVerticalHeaderLabels(vnames);
        ui.tPorogsValues->setHorizontalHeaderLabels(hnames);
        ui.tPorogsValues->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
//        ui.tPorogsValues->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
        ui.tPorogsValues->blockSignals(false);
    }
}

void SPRPorogsWidget::viewChange()
{
    QLineEdit *le = qobject_cast<QLineEdit*>(sender());
    double value = le->text().toDouble();
    QTableWidget *tw = sender()->property("tw").value<QTableWidget*>();
    int row = le->property("row").toInt();
    int col = le->property("col").toInt();
    model->porogs[col][row]->setData(value);
    emit tableChange(tw, row, col);
}

ISPRModelData *SPRPorogsWidget::getModel()
{
    return model;
}

