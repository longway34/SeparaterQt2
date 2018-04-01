#ifndef TCPTIMEOUTWIGGET_H
#define TCPTIMEOUTWIGGET_H

#include "ui_tcptimeoutwigget.h"

class TCPTimeOutWigget : public QWidget
{
    Q_OBJECT

public:
    explicit TCPTimeOutWigget(QWidget *parent = 0);


public slots:
    void onRangeChange(int step, int all);
    void onCommandComplite(int){
        this->close();
    }
    void setTitle(QString _title){
        this->setWindowTitle(_title);
    }
    void setMessage(QString _message){
        ui.lMessage->setText(_message);
    }
private:
    Ui::TCPTimeOutWigget ui;
};

#endif // TCPTIMEOUTWIGGET_H
