#include "maintabwidget.h"
#include "mainwindow.h"

#include <QApplication>
#include <QMainWindow>

#include "models/sprspectrumlistitemsmodel.h"
#include "models/sprelementsmodel.h"

#include <QDomDocument>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{


//    SPRElementsModel *model;

//    QDomDocument doc;
//    QFile in("Separator.xml");
//    if(in.open(QIODevice::ReadOnly)){
//        if(doc.setContent(&in)){


//            model = new SPRElementsModel(&doc);

//            model->IModelVariable::store();
//            in.close();

//            QFile out("Separator1.xml");
//            if(out.open(QIODevice::WriteOnly)){
//            QTextStream stream( &out );
//              stream << doc.toString();

//              out.close();
//            }



//        }

//    }




    QApplication a(argc, argv);

//    MainWindow w(&doc);
//    w.show();

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
