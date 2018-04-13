#include "maintabwidget.h"

#include <QApplication>

#include "models/sprspectrumlistitemsmodel.h"

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

    SPRSpectrumListItemsModel *model;

    QDomDocument doc;
    QFile in("СРФ1.xml");
    if(in.open(QIODevice::ReadOnly)){
        if(doc.setContent(&in)){


            model = new SPRSpectrumListItemsModel(&doc);

            model->store();
            in.close();

        }

    }




    QApplication a(argc, argv);

//    MainWindow w(&doc);
//    w.show();

    MainTabWidget tw("Separator.xml", nullptr);
    tw.show();

    return a.exec();
}
