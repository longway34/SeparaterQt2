/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ServerConnect.cpp
 * Author: longway
 * 
 * Created on 9 февраля 2018 г., 17:06
 */

#include "ServerConnect.h"

ServerConnect::~ServerConnect() {
}

void ServerConnect::setPort(uint16_t port) {
    this->port = port;
}

uint16_t ServerConnect::getPort() const {
    return port;
}

void ServerConnect::setName(QString name) {
    this->name = name;
}

QString ServerConnect::getName() const {
    return name;
}

void ServerConnect::addSendCommand(ITCPCommand* _sender){
    this->mutex.lock();
    sendData.push_back(_sender);
    this->mutex.unlock();
    
    emit addData();
}

void ServerConnect::clearQueue(){
    mutex.lock();
    sendData.clear();
    readAll();
    flush();
//    disconnectFromHost();

    mutex.unlock();
}

void ServerConnect::queueComplite(){
   while(!sendData.empty()){
       mutex.lock();
       current = sendData.front();
       sendData.pop_front();
       if(state() != QAbstractSocket::SocketState::ConnectedState){
           connectToHost(name, port);
           if(!waitForConnected(500)){
               mutex.unlock();
               emit serverConnectTimeOutError(current);
               return;
           }
       }
//       while(bytesAvailable()){
//           readAll();
//       }
        write(current->toQueue());
        if(waitForReadyRead(500) || bytesAvailable()){
            onReadyRead();
        } else {
            mutex.unlock();
            emit serverReadWriteTimeOutError(current);
        }
   } 
}

void ServerConnect::onReadyRead(){
    replay.clear();
    while(bytesAvailable()){
        replay.append(readAll());
    }
    mutex.unlock();
    if(current->getCommand() == getstate){
        changeRemoteState(replay);
    }
    current->setReplayData(replay);
//    emit commandComlite(current);
}

