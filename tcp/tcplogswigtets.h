#ifndef TCPLOGSWIGTETS_H
#define TCPLOGSWIGTETS_H

#include "ui_tcplogswigtets.h"
#include "tcp/ITCPCommand.h"

class TCPLogsWigtets : public QWidget
{
    Q_OBJECT
protected:
    QString onLogsCommand(ITCPCommand *command);
    void onLogsCommand(QString msg, QColor _color = QColor());


    QTextCharFormat textCharFormatDefault;
    QTextCharFormat textCharFormatAther;

public:
    explicit TCPLogsWigtets(QWidget *parent = 0);
    Ui::TCPLogsWigtets ui;

public slots:
    void onClear(bool);
    void onLogsCommand(ITCPCommand *command, QString _prefix);
    void onErrorLogsCommand(ITCPCommand *command, QString _prefix);
private:
};

#endif // TCPLOGSWIGTETS_H
