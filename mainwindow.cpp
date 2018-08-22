#include "mainwindow.h"
#include <QDialog>
#include <QInputDialog>
#include <QApplication>
#include <QDesktopWidget>

#include <QMessageBox>

MainWindow::MainWindow(QString fName, QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);

//#ifdef WIN32
//    bool y = QDir::setCurrent("F:\\Projects\\Separator\\");
//#else
//    bool y = QDir::setCurrent("/home/longway/");
//#endif

    isMasterMode = true;

    ui.wMainTabWidget->setDoc(fName);
    ui.wMainTabWidget->setLogWidget(ui.wLog);

    connect(ui.actMasterMode, SIGNAL(changed()), this, SLOT(onChangeMasterMode()));

    ui.wMainTabWidget->setIsMasterMode(false);
    QRect r = QApplication::desktop()->screenGeometry();
    this->resize(r.width() -10, r.height() -20);

    this->setWindowFlags(Qt::Window
                         | Qt::WindowMinimizeButtonHint
                         | Qt::WindowMaximizeButtonHint
                         );

    connect(ui.actExit, SIGNAL(triggered(bool)), this, SLOT(onCloseCommand(bool)));
    connect(ui.actAbout, SIGNAL(triggered(bool)), this, SLOT(onAbout(bool)));

    about = new AboutDialog(nullptr);
}

void MainWindow::onChangeMasterMode(){
    if(sender() == ui.actMasterMode){
        QAction *act = (QAction*)sender();
        if(act->isChecked()){
            bool isOk;
            QString pass = QInputDialog::getText(nullptr, tr("Ввод пароля"), tr("Введите пароль мастера..."), QLineEdit::Password, QString(), &isOk);

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

void MainWindow::onAbout(bool){
    about->show();
}

void MainWindow::onCloseCommand(bool)
{
    SPRMainModel *model = ui.wMainTabWidget->getModel();
    if(model){
        QList<ServerConnectState> disableStates = {
            spr_state_separated, spr_state_pitatel_on, spr_state_rasklad_on, spr_state_rentgen_on, spr_state_separator_on
        };
        bool res = false;
        foreach(ServerConnectState state, disableStates){
            res |= model->getServer()->isState(state);
            if(res){
                break;
            }
        }
        if(res){
            QMessageBox::warning(nullptr, tr("Ошибка завершения программы"),tr("Завершить программу, в данный момент, невозможно\nПеред завершением программы надо включить сепарацию и оборудование..."));
        } else {
            exit(0);
        }
    }

}
