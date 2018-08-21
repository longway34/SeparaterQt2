#include "mainwindow.h"
#include <QDialog>
#include <QInputDialog>
#include <QApplication>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);

#ifdef WIN32
    bool y = QDir::setCurrent("F:\\Projects\\Separator\\");
#else
    bool y = QDir::setCurrent("/home/longway/");
#endif

    isMasterMode = true;

    ui.wMainTabWidget->setDoc("Separator1.xml");
    ui.wMainTabWidget->setLogWidget(ui.wLog);

    connect(ui.actMasterMode, SIGNAL(changed()), this, SLOT(onChangeMasterMode()));

    ui.wMainTabWidget->setIsMasterMode(false);
    QRect r = QApplication::desktop()->screenGeometry();
    this->resize(r.width() -10, r.height() -20);
}

void MainWindow::onChangeMasterMode(){
    if(sender() == ui.actMasterMode){
        QAction *act = (QAction*)sender();
        if(act->isChecked()){
            bool isOk;
            QString pass = QInputDialog::getText(0, tr("Ввод пароля"), tr("Введите пароль мастера..."), QLineEdit::Password, QString(), &isOk);

            if(isOk && pass == "3912"){
                ui.actOpenConfig->setEnabled(true);
                ui.actSave->setEnabled(true);
                ui.actMasterMode->setChecked(true);

                ui.wMainTabWidget->setIsMasterMode(true);
                ui.wLog->onLogsCommand(nullptr, tr("Переход в режим мастера..."));
            } else {
                ui.actMasterMode->setChecked(false);
                ui.actOpenConfig->setEnabled(false);
                ui.actSave->setEnabled(false);

                ui.wMainTabWidget->setIsMasterMode(false);
                ui.wLog->onErrorLogsCommand(nullptr, tr("Пароль мастера не верен..."));
            }
        } else {
            ui.actMasterMode->setChecked(false);
            ui.actOpenConfig->setEnabled(false);
            ui.actSave->setEnabled(false);

            ui.wMainTabWidget->setIsMasterMode(false);
            ui.wLog->onLogsCommand(nullptr, tr("Переход в режим с ограниченными возможностями..."));
        }

    }
}
