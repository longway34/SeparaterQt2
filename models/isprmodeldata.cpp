#include <QDebug>
#include "isprmodeldata.h"
#include <QFile>

QDomDocument *ISPRModelData::getDoc() const
{
    return doc;
}

void ISPRModelData::setDoc(QDomDocument *_doc, IModelVariable *parent)
{
    doc = _doc;
    if(parent){
        connect(parent, SIGNAL(goStore()), this, SLOT(saveAs()));
    }
}

void ISPRModelData::setDoc(QString fname, IModelVariable *parent)
{
    if(!fname.isEmpty()) {fileName = fname;}
    QFile in(fileName);
    if(in.open(QIODevice::ReadOnly)){
        if(!document.setContent(&in)){
            qDebug() << "Error!!! read content from " << fname << "new file created now...";
            doc = new QDomDocument("SEPARATOR");

            document = *doc;
        }
    }
    setDoc(&document, parent);
}


ISPRModelData::ISPRModelData(QString fname, IModelVariable *parent): IModelVariable(nullptr, parent)
{
    setDoc(fname, parent);
}

ISPRModelData::ISPRModelData(QDomDocument *_doc, IModelVariable *parent): IModelVariable(_doc, parent)
{
    setDoc(_doc, parent);
}

void ISPRModelData::saveAs(QString fname)
{
    emit doStore();
    if(!fname.isEmpty()){ fileName = fname; }
        QFile fout(fileName);
        if(fout.open(QIODevice::WriteOnly)){
            QTextStream out(&fout);
            out << doc->toString();
            fout.close();
        }
}

