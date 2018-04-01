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

TCPCommandSet::TCPCommandSet(){
    command = nocommand;
}

TCPCommandSet::~TCPCommandSet() {
}

void TCPCommandSet::go(TCPCommand* _command){
    if(_command){
        int num = _command->getNum();
        if(num < commandSet.size() - 1){
            commandSet[num+1]->send(server);
        } else {
            emit commandComplite(this);
        }
    } else {
        if(commandSet.size() > 0){
            commandSet[0]->send(server);
        } else {
            emit commandComplite(this);
        }
    }
}
