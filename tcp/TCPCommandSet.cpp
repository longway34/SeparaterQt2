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

TCPCommandSet::TCPCommandSet(ServerConnect *_server, TCPTimeOutWigget *_widget, QVector<TCPCommand *> _vcomm): TCPTimeOutCommand(timeoutcommand, 0, 1, _widget, "", ""){
    server = _server;
    replayData = QByteArray(1, '\0');
    addCommand(_vcomm);
}

TCPCommandSet::~TCPCommandSet() {
}

TCPCommandSet *TCPCommandSet::addCommand(TCPCommand *_comm){
    _comm->setNum(commandSet.size());
    commandSet.push_back(_comm);
//    static const QMetaMethod commandCompliteSignal = QMetaMethod::fromSignal(&TCPCommand::commandComplite);
//    if(!isSignalConnected(commandCompliteSignal)){
        connect(_comm, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(go(TCPCommand*)));
//    }
    return this;
}

TCPCommandSet *TCPCommandSet::addCommand(QVector<TCPCommand *> _vcomm){
    for(int i=0; i<_vcomm.size(); i++){
        addCommand(_vcomm[i]);
    }
    return this;
}

TCPCommandSet *TCPCommandSet::addCommand(QVector<EnumCommands> _ecomm){
    for(int i=0; i<_ecomm.size(); i++){
        addCommand(_ecomm[i]);
    }
    return this;
}

TCPCommandSet *TCPCommandSet::addCommand(EnumCommands ecommand){
    return addCommand(new TCPCommand(ecommand));
}

void TCPCommandSet::send(ServerConnect *_server){
    server = _server;
    if(server){
        server->timerStop();
    }
    go();
}

TCPCommand *TCPCommandSet::setSendData(QByteArray _sendData, EnumCommands _command)
{
    return setSendData(_sendData.data(), _sendData.size(), _command);
}

TCPCommand *TCPCommandSet::setSendData(void *data, int len, EnumCommands _command)
{
    TCPCommand *_comm;
    if(_command == lastcommand){
        _comm = commandSet.last();
    } else {
        _comm = findCommands(_command).last();
    }
    if(_comm){
        _comm->setSendData(data, len);
    }
    return this;
}

TCPCommand *TCPCommandSet::addSendData(QByteArray sendData, EnumCommands _command){
    return addSendData((void*)sendData.constData(), sendData.size(), _command);
}

TCPCommand *TCPCommandSet::addSendData(void *data, uint len, EnumCommands _command){
    TCPCommand *_comm;
    if(_command == lastcommand){
        _comm = commandSet.last();
    } else {
        _comm = findCommands(_command).last();
    }
    if(_comm){
        _comm->addSendData(data, len);
    }
    return this;
}

TCPCommand *TCPCommandSet::findCommand(TCPCommand *_command){
    for(int i=0; i< commandSet.size(); i++){
        if(commandSet[i] == _command){
            return commandSet[i];
        }
    }
    return nullptr;
}

void TCPCommandSet::go(TCPCommand* _command){
    if(_command){

        int num = _command->getNum();
        if(num < commandSet.size() - 1){
            if(isCommamdCompare(_command)){
                commandSet[num+1]->send(server);
            } else {
                emit commandNotComplite(_command);
                server->timerStart();
            }
        } else {
            if(server){
                server->timerStart();
            }
            if(isCommamdCompare(_command)){
                emit commandComplite(this);
            } else {
                emit commandNotComplite(_command);
            }
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


bool TCPCommandSet::isCommandSet()
{
    return true;
}
