/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TCPCommanSet.cpp
 * Author: longway
 * 
 * Created on 10 февраля 2018 г., 18:32
 */

#include "TCPCommandSet.h"

#include <QDebug>

TCPCommandSet::TCPCommandSet():TCPTimeOutCommand(){
    command = nocommand;
}

TCPCommandSet::~TCPCommandSet() {
}

void TCPCommandSet::go(TCPCommand* _command){
    if(_command){

        qDebug() << "command: "<<QString::number(_command->getCommand(),16)<< "; send:" << _command->getSendData().toHex(':')<< "; res: "<<_command->getReplayData().toHex(':');

        int num = _command->getNum();
        if(num < commandSet.size() - 1){
            if(isCommamdCompare(_command)){
                commandSet[num+1]->send(server);
            } else {
                emit commandComplite(_command);
                server->timerStart();
            }
        } else {
            if(server){
                server->timerStart();
            }
            emit commandComplite(this);
        }
    } else {
        if(commandSet.size() > 0){
            server->timerStop();
            commandSet[0]->send(server);
        } else {
            if(server){
                server->timerStart();
            }
            emit commandComplite(this);
        }
    }
}

void TCPCommandSet::onCommandNotComplite(TCPCommand *_command)
{
    emit commandNotComplite(_command);
}


void TCPCommandSet::setLogWidget(TCPLogsWigtets *value)
{
    ITCPCommand::setLogWidget(value);
    for(int i=0; i<commandSet.size(); i++){
        commandSet[i]->setLogWidget(value);
    }
}

bool TCPCommandSet::noErrors()
{
    bool res = true;
    for(int i=0; i<commandSet.size(); i++){
        res &= commandSet[i]->noErrors();
    }
    return res;
}
