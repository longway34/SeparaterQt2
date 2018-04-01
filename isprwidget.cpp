#include <QLineEdit>
#include <QTableWidget>
#include <QFileInfo>
#include <QFile>

#include "isprwidget.h"


ISPRWidget::ISPRWidget()
{
}

void ISPRWidget::setDoc(QString _fName)
{
    QFile in(_fName);
    if(in.open(QIODevice::ReadOnly)){
        if(document.setContent(&in)){
            QFileInfo fi(_fName);
            docFileName = fi.absoluteFilePath();
            docFilePath = fi.absolutePath();
            doc = &document;
            setDoc(doc);
        }
        in.close();
    }
}

void ISPRWidget::setDoc(QDomDocument *_doc)
{
    doc = _doc;
}

ISPRModelData *ISPRWidget::setModel(ISPRModelData *data)
{
    return data;
}

QLineEdit *ISPRWidget::setNumberCell(QTableWidget *table, int row, int col, int value, int min, int max, const QString toolType)
{
   QLineEdit *leNum = new QLineEdit(QString::number(value), table);
   leNum->setFrame(false); leNum->setAlignment(Qt::AlignCenter);
   leNum->setValidator(new QIntValidator(min, max, leNum/*table->item(row, col)->*/));
   table->setCellWidget(row, col, leNum);
   leNum->setProperty("row", row); leNum->setProperty("col", col);

   QVariant qv_tw;
   qv_tw.setValue<QTableWidget*>(table);
   leNum->setProperty("tw", qv_tw);

   leNum->setObjectName("leTable");
   leNum->setToolTip(toolType);
   return leNum;
}

