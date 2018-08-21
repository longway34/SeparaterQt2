/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ITCPCommand.cpp
 * Author: longway
 * 
 * Created on 9 февраля 2018 г., 17:23
 */

#include "TCPCommand.h"

TCPCommand::TCPCommand() :ITCPCommand(){
    command = nocommand;
}

TCPCommand::~TCPCommand(){
}

TCPCommand *TCPCommand::setSendData(QByteArray _sendData, EnumCommands _command){
    return setSendData(_sendData.data(), _sendData.size(), _command);
}
TCPCommand *TCPCommand::setSendData(void *data, uint len, EnumCommands){
    sendData = QByteArray((char*)data, len);
    return this;
}

TCPCommand *TCPCommand::addSendData(QByteArray _sendData, EnumCommands _command) {
    return addSendData(_sendData.data(), _sendData.size(), _command);
}
TCPCommand *TCPCommand::addSendData(void *data, uint len, EnumCommands){
    sendData.append((char*)data, len);
    return this;
}

void TCPCommand::clear(){
    for(int i=0; i<commandSet.size(); i++){
        commandSet[i]->clear();
        delete commandSet[i];
        commandSet[i] = nullptr;
    }
    commandSet.clear();
}

void TCPCommand::onCommandNotComplite(TCPCommand *_command)
{
    emit commandNotComplite(_command);
}


bool TCPCommand::isRentgenReady(QByteArray result, uint16_t *mkv, uint16_t *mka, uint8_t *err){
    bool res = false;
    if(mka) *mka = 0; if(mkv) *mkv = 0;
    uint16_t _mka = 0, _mkv = 0;
    uint8_t _err = (result.size() != 5) ? 1 : *result.left(1).constData();
    if(!_err){ // length result is correct or command complite
        QByteArray arr = result.right(4);
        memcpy(&_mka, arr.constData(), 2);
        memcpy(&_mkv, arr.constData()+2, 2);
        if(mka) *mka = _mka; if(mkv) *mkv = _mkv;
        if(_mka < 0x600 || _mka > 0x700 || _mkv < 0x600 || _mkv > 0x700){
            _err = 2;
        } else {
            res = true;
        }
    }
    if(err) *err = _err;
    return res;
}

bool TCPCommand::isCommandSet()
{
    return false;
}
