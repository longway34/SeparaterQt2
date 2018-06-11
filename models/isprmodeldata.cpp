#include <QDebug>
#include "isprmodeldata.h"
#include <QFile>
#include <QFileInfo>

QDomDocument *ISPRModelData::getDoc() const
{
    return doc;
}

void ISPRModelData::setDoc(QDomDocument *_doc, IModelVariable *parent)
{
    if(_doc){
        doc = _doc;
        document = *doc;
    }
    setMVParent(parent);
}

void ISPRModelData::setDoc(QString fname, IModelVariable *parent)
{
    fileName = "";
    if(!fname.isEmpty()) {fileName = fname;}
    QFile in(fileName);
    if(in.open(QIODevice::ReadOnly)){
        QFileInfo fi(fname);
        fileName = fi.absoluteFilePath();
        if(!document.setContent(&in)){
            qDebug() << "Error!!! read content from " << fname << "new file created now...";
            doc = new QDomDocument("SEPARATOR");

            document = *doc;
        }
    }
    setDoc(&document, parent);
}


QString ISPRModelData::getFileName() const
{
    return fileName;
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
            QTextCodec *codec = QTextCodec::codecForName("UTF-8");
            struct QTextCodec::ConverterState state;

            QTextCodec::setCodecForLocale(codec);

            qint64 len = strlen(doc->toString().toStdString().c_str());
            QByteArray baOut = codec->fromUnicode(doc->toString().unicode(), len, &state);

            QTextStream out(&fout);
//            out << doc->toString();
            out << baOut.left(len+2);
            fout.close();
        }
}

