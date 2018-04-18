#include "maintabwidget.h"

#include <QApplication>

#include "models/sprspectrumlistitemsmodel.h"
#include "models/sprelementsmodel.h"

#include <QDomDocument>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{

#ifdef WIN32
    bool y = QDir::setCurrent("F:\\Projects\\Separator\\");
#else
    bool y = QDir::setCurrent("/home/longway/");
#endif

    SPRElementsModel *model;

    QDomDocument doc;
    QFile in("Separator.xml");
    if(in.open(QIODevice::ReadOnly)){
        if(doc.setContent(&in)){


            model = new SPRElementsModel(&doc);

            model->store();
            in.close();

            QFile out("Separator1.xml");
            if(out.open(QIODevice::WriteOnly)){
            QTextStream stream( &out );
              stream << doc.toString();

              out.close();
            }



        }

    }




    QApplication a(argc, argv);

//    MainWindow w(&doc);
//    w.show();

    MainTabWidget tw("Separator.xml", nullptr);
    tw.show();

    return a.exec();
}
