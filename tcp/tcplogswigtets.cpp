#include "tcp/tcplogswigtets.h"

TCPLogsWigtets::TCPLogsWigtets(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.bClearLogs, SIGNAL(clicked(bool)), this, SLOT(onClear(bool)));
}

void TCPLogsWigtets::onClear(bool){
    ui.ptLogs->clear();
}

void TCPLogsWigtets::onLogsCommand(TCPCommand *command){
    QString msg = "command "+QString::number((int8_t(command->getCommand())))+" complite whith result ";
    msg += command->getReplayData().toHex(':');

    ui.ptLogs->appendPlainText(msg);
}

void TCPLogsWigtets::onLogsCommand(TCPCommand *command, QString msg){
    ui.ptLogs->appendPlainText(msg);
    onLogsCommand(command);
}
