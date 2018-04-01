#ifndef TCPLOGSWIGTETS_H
#define TCPLOGSWIGTETS_H

#include "ui_tcplogswigtets.h"
#include "tcp/TCPCommand.h"

class TCPLogsWigtets : public QWidget
{
    Q_OBJECT

public:
    explicit TCPLogsWigtets(QWidget *parent = 0);
    Ui::TCPLogsWigtets ui;

public slots:
    void onClear(bool);
    void onLogsCommand(TCPCommand *command);
    void onLogsCommand(TCPCommand *command, QString msg);
private:
};

#endif // TCPLOGSWIGTETS_H
