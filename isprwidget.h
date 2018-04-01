#ifndef ISPRWIDGET_H
#define ISPRWIDGET_H

#include <QTableWidget>
#include <QString>
#include <QLineEdit>
#include <QValidator>
#include "models/isprmodeldata.h"

class ISPRWidget
{
protected:
    QString docFileName;
    QString docFilePath;

    QDomDocument document;
    QDomDocument *doc;

public:
    ISPRWidget();
    virtual void setDoc(QString _fName);
    virtual void setDoc(QDomDocument *_doc);

    virtual void widgetsShow()=0;

//    virtual void doShow(){}
    virtual ISPRModelData *setModel(ISPRModelData* data=nullptr);
    virtual ISPRModelData *getModel()=0;
//    virtual void cellLeChanget(QTableWidgt *table, QLineEdit *le, int x, int y)=0;
protected:
    QLineEdit* setNumberCell(QTableWidget *table, int row, int col, int value, int min, int max, const QString toolType);
    virtual void viewChange(){}
    virtual void viewChange(QColor){}
    virtual void viewChange(int){}
    virtual void viewChange(double){}
    virtual void viewChange(bool){}
    virtual void viewChange(int, int){}
    virtual void viewChange(QTableWidget*, int, int){}
};

#endif // ISPRWIDGET_H
