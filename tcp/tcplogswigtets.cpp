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

void TCPLogsWigtets::onErrorLogsCommand(QString msg, TCPCommand *command){
    QDateTime dt(QDateTime::currentDateTime());
    QString sdt = dt.toString("hh:mm:ss");
    QTextCharFormat old = ui.ptLogs->currentCharFormat();
    QTextCharFormat tf = old;
    tf.setForeground(QBrush(QColor(255, 128, 128)));
    ui.ptLogs->setCurrentCharFormat(tf);
    if(command){
        onLogsCommand(command);
    }
    ui.ptLogs->appendPlainText(sdt+": "+msg);
    ui.ptLogs->setCurrentCharFormat(old);
}

void TCPLogsWigtets::onLogsCommand(TCPCommand *command){
    if(command){
        QDateTime dt(QDateTime::currentDateTime());
        QString sdt = dt.toString("hh:mm:ss ");
        QString msg = "command "+QString::number((int8_t(command->getCommand())))+"; result: ";
        msg += command->getReplayData().toHex(':');

        ui.ptLogs->appendPlainText(sdt+": "+msg);
    }
}

void TCPLogsWigtets::onLogsCommand(TCPCommand *command, QString msg){
    QDateTime dt(QDateTime::currentDateTime());
    QString sdt = dt.toString("hh:mm:ss");
    if(command){
        onLogsCommand(command);
    }
    ui.ptLogs->appendPlainText(sdt+": "+msg);
}

void TCPLogsWigtets::onLogsCommand(QString msg){
    QDateTime dt(QDateTime::currentDateTime());
    QString sdt = dt.toString("hh:mm:ss");
    ui.ptLogs->appendPlainText(sdt+": "+msg);
}
