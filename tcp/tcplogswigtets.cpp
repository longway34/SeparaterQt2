#include "tcp/tcplogswigtets.h"
#include <QDateTime>
#include <QTextCharFormat>
#include <QBrush>

TCPLogsWigtets::TCPLogsWigtets(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.bClearLogs, SIGNAL(clicked(bool)), this, SLOT(onClear(bool)));
}

void TCPLogsWigtets::onClear(bool){
    ui.ptLogs->clear();
}

void TCPLogsWigtets::onErrorLogsCommand(ITCPCommand *command, QString _prefix){
    QString msg = _prefix + onLogsCommand(command);
    onLogsCommand(msg, QColor(Qt::red));
}

QString TCPLogsWigtets::onLogsCommand(ITCPCommand *command){
//    if(command){
    QString ret = "";
//    QDateTime dt(QDateTime::currentDateTime());
//    QString sdt = dt.toString("hh:mm:ss ");

//        ret += sdt;

        if(command){
            QString msg = QString(tr("\nCommand 0x%1; send: %2; result: %3 ")).
                    arg(QString::number(command->getCommand(), 16)).
                    arg(QString::fromStdString(command->getSendData().toHex(':').toStdString())).
                    arg(QString::fromStdString(command->getReplayData().toHex(':').toStdString()));
            ret += msg ;

        }
        return ret;
//        QString msg = "command 0x"+QString::number((int8_t(command->getCommand()), 16))+"; result: ";
//        msg += command->getReplayData().toHex(':');

//        ui.ptLogs->appendPlainText(sdt+": "+msg);
//    }
}

void TCPLogsWigtets::onLogsCommand(ITCPCommand *command, QString _prefix){
    QString msg = _prefix + onLogsCommand(command);
    onLogsCommand(msg);
//    ui.ptLogs->appendPlainText(sdt+": "+msg);
}

void TCPLogsWigtets::onLogsCommand(QString msg, QColor _color){
    QDateTime dt(QDateTime::currentDateTime());
    QString sdt = dt.toString("hh:mm:ss");
    QTextCharFormat old = ui.ptLogs->currentCharFormat();;
    if(_color.isValid()){
        QTextCharFormat tf = old;
        tf.setForeground(QBrush(_color));
        ui.ptLogs->setCurrentCharFormat(tf);
    }
//    if(command){
//        onLogsCommand(command);
//    }
    ui.ptLogs->appendPlainText(sdt+": "+msg);
    if(_color.isValid()){
        ui.ptLogs->setCurrentCharFormat(old);
    }
//    QDateTime dt(QDateTime::currentDateTime());
//    QString sdt = dt.toString("hh:mm:ss");
//    ui.ptLogs->appendPlainText(sdt+": "+msg);
}
