/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TCPTimeOutCommand.cpp
 * Author: longway
 * 
 * Created on 10 февраля 2018 г., 15:06
 */

#include "TCPTimeOutCommand.h"

TCPTimeOutCommand::TCPTimeOutCommand(): TCPCommand() {
    parts = 0;
    timeout = 0;
    command = nocommand;
}

TCPTimeOutCommand::TCPTimeOutCommand(EnumCommands _command, uint _timeout, uint _parts, TCPTimeOutWigget *_widget, QString _title, QString _message):
    TCPCommand(_command), widget(_widget), parts(_parts), timeout(_timeout), title(_title), message(_message)
{
        connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
//    if(widget){
//        connect(this, SIGNAL(compliteParts(int, int)), widget, SLOT(onRangeChange(int, int)));
//    }
}

void TCPTimeOutCommand::setReplayData(QByteArray replayData){
    this->replayData = replayData;
    if(timeout > 0 && parts >= 1){
        timer.setInterval(timeout / parts);
        timer.start();
        countParts = 0;
        if(widget){
            widget->setWindowTitle(title);
            widget->setMessage(message);
            
            widget->show();
            widget->onRangeChange(countParts, parts);
        }
    } else {
        emit commandComplite(this);
    }
}

void TCPTimeOutCommand::onTimeOut(){
    countParts++;
    if(widget){
        widget->onRangeChange(countParts, parts);
    }
    if(countParts >= parts){
        if(widget){
            widget->close();
        }
        timer.stop();
        emit commandComplite(this);
    }
}

TCPTimeOutCommand::~TCPTimeOutCommand() {
}


void TCPTimeOutCommand::setTimeOut(int _timeout)
{
    timeout = _timeout;
    countParts = 0;
}
