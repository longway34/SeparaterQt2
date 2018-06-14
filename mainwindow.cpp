#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);

#ifdef WIN32
    bool y = QDir::setCurrent("F:\\Projects\\Separator\\");
#else
    bool y = QDir::setCurrent("/home/longway/");
#endif

    ui.wMainTabWidget->setDoc("Separator1.xml");
    ui.wMainTabWidget->setLogWidget(ui.wLog);
}
