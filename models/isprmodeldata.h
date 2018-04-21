#ifndef ISPRMODELDATA_H
#define ISPRMODELDATA_H

#include <QObject>
#include <QWidget>
#include <QList>
#include <QDomDocument>
#include <QFile>
#include <iostream>
#include "models/imodelvariable.h"

//#include "models/isprvariable.h"

class ISPRModelData : public IModelVariable
{
    Q_OBJECT

protected:
    QDomDocument document;
    QString fileName;
//    QDomDocument *doc;
//    QDomElement root;

public:
    ISPRModelData(){}
    ISPRModelData(QString fname, IModelVariable *parent = nullptr);
    ISPRModelData(QDomDocument *_doc, IModelVariable *parent = nullptr);

    QDomDocument *getDoc() const;
    void setDoc(QDomDocument *_doc, IModelVariable *parent = nullptr);
    void setDoc(QString fname, IModelVariable *parent = nullptr);

public slots:
    void saveAs(QString fname = "");
};

#endif // ISPRMODELDATA_H
