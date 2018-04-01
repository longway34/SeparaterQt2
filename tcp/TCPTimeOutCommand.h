/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TCPTimeOutCommand.h
 * Author: longway
 *
 * Created on 10 февраля 2018 г., 15:06
 */

#ifndef TCPTIMEOUTCOMMAND_H
#define TCPTIMEOUTCOMMAND_H

#include "TCPCommand.h"
#include "tcptimeoutwigget.h"
#include <QTimer>


class TCPTimeOutCommand : public TCPCommand{
    
    Q_OBJECT

protected:
    TCPTimeOutWigget *widget;
    QTimer timer;
    
    uint parts;
    uint countParts;
    uint timeout;
    
    QString title;
    QString message;
    
public:
    TCPTimeOutCommand();
    TCPTimeOutCommand(EnumCommands _command, uint _timeout = 0, uint parts = 1, TCPTimeOutWigget *_widget = NULL, QString _title = "", QString _message = "");
    virtual ~TCPTimeOutCommand();
    virtual void setReplayData(QByteArray replayData);

    void setWidgetTitleMessage(QString _title, QString _msg){
         title = _title;
         message = _msg;
    }
private:

public slots:
    void onTimeOut();
    
signals:
    void copliteParts(int, int);

    // TCPCommand interface
public:
    virtual void setTimeOut(int);
};



#endif /* TCPTIMEOUTCOMMAND_H */

