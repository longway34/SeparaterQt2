#include "maintabwidget.h"
#include "mainwindow.h"

#include <QApplication>
#include <QMainWindow>

#include "models/sprspectrumlistitemsmodel.h"
#include "models/sprelementsmodel.h"

#include <QDomDocument>
#include <QFile>
#include <QDir>


void myMyssageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){
    switch(type){
    case QtInfoMsg:
        std::cerr << QString("Info: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg(context.function).toStdString();
        break;
    case QtDebugMsg:
//        std::cerr << QString("Debug: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg(context.function).toStdString();
        std::cerr << QString(msg).toStdString() << std::endl;
        break;
    case QtWarningMsg:
        std::cerr << QString("Warn: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg(context.function).toStdString();
        break;
    case QtCriticalMsg:
        std::cerr << QString("Crit: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg(context.function).toStdString();
        break;
    case QtFatalMsg:
        std::cerr << QString("Fatal: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg(context.function).toStdString();
        abort();
    }
}

int main(int argc, char *argv[])
{

    qInstallMessageHandler(myMyssageOutput);

    QApplication a(argc, argv);

//    MainWindow w(&doc);
//    w.show();

    MainWindow w;
    w.show();

    return a.exec();
}
