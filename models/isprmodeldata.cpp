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
        connect(parent, SIGNAL(goStore()), this, SLOT(store()));
    }
}

void ISPRModelData::setDoc(QString fname, IModelVariable *parent)
{
    QFile in(fname);
    if(in.open(QIODevice::ReadOnly)){
        if(!document.setContent(&in)){
            qDebug() << "Error!!! read content from " << fname << "new file created now...";
            doc = new QDomDocument("SEPARATOR");

            document = *doc;
        }
    }
    setDoc(&document, parent);
}


ISPRModelData::ISPRModelData(QString fname, IModelVariable *parent):doc(nullptr)
{
    setDoc(fname, parent);
}

ISPRModelData::ISPRModelData(QDomDocument *_doc, IModelVariable *parent): doc(nullptr)
{
    setDoc(_doc, parent);
}

void ISPRModelData::store(QString fname)
{
    store();
    if(!fname.isEmpty()){
        QFile fout(fname);
        if(fout.open(QIODevice::WriteOnly)){
            QTextStream out(&fout);
            out << doc->toString();
            fout.close();
        }
    }
}

void ISPRModelData::store(){
    emit doStore();
}
