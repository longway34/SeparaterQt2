/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ITCPCommand.h
 * Author: longway
 *
 * Created on 9 февраля 2018 г., 17:23
 */

#ifndef TCPCOMMAND_H
#define TCPCOMMAND_H

#include <stdint.h>
#include <QDebug>
#include <QObject>
#include "ITCPCommand.h"
#include "ServerConnect.h"

class TCPCommand: public ITCPCommand {
    
    Q_OBJECT
    
protected:
    QVector<TCPCommand*> commandSet;

public:
    TCPCommand();
    TCPCommand(EnumCommands _command): ITCPCommand(){
        command = _command;
    }
    virtual ~TCPCommand();
    
    virtual void clear(){
        for(int i=0; i<commandSet.size(); i++){
            commandSet[i]->clear();
            delete commandSet[i];
        }
        commandSet.clear();
    }

    virtual void setReplayData(QByteArray replayData){
        this->replayData = replayData;

        qDebug() << "command: "<<QString::number(getCommand(),16)<< "; send:" << getSendData().toHex(':')<< "; res: "<<getReplayData().toHex(':');

        if(noErrors()){
            emit commandComplite(this);
        } else {
            emit commandNotComplite(this);
        }
    }

    virtual QByteArray getReplayData() {
        return replayData;
    }
    virtual int getReplayDataSize(){
        return replayData.size();
    }

    virtual void setSendData(QByteArray sendData) {
        this->sendData = sendData;
    }

    virtual void setSendData(void *data, uint len){
        QByteArray ba;
        ba.append((char*)data, len);
        setSendData(ba);
    }

    QByteArray getSendData() {
        return sendData;
    }
    
    virtual void send(ServerConnect *server){
        if(command != nocommand && command != timeoutcommand && command != lastcommand){
            server->addSendCommand(this);
        } else {
            setReplayData(QByteArray::fromRawData("\x0", 1));
        }
    }
    
    virtual void setTimeOut(int){
    }
    
    virtual bool noErrors(){
        if(replayData.size()>0){
            return replayData[0] == '\0';
        } else {
            return true;
        }
    }
    virtual int getErrors(){
        if(replayData.size()>0){
            return int(replayData[0]);
        }
        return -1;
    }
    virtual int getState(){
        if(replayData.size()>1){
            return int(replayData[1]);
        }
        return -1;
    }
    virtual QByteArray getResult(){
        QByteArray res;
        if(replayData.size() > 1){
            for(int i=1; i< replayData.size(); i++)
            res += replayData[i];
        }
        return res;
    }

    virtual EnumCommands getDataType(){
        return command;
    }
    virtual QVector<TCPCommand*> findCommands(EnumCommands _command=lastcommand){
        if(_command == lastcommand){
            return QVector<TCPCommand*>({this});
        } else {
            QVector<TCPCommand*> res;
            if(_command == this->command){
                res.push_back(this);
            }
            for(int i=0; i<commandSet.size();i++){
                TCPCommand *comm = commandSet[i];
//                if(comm->getCommand() == _command){
                    QVector<TCPCommand*> vres = comm->findCommands(_command);
                    for(int v=0; v<vres.size();v++){
                        res.push_back(vres[v]);
                    }
//                }
            }
            return res;
        }
    }

public slots:
    virtual void go(TCPCommand *_command = NULL){
    }

private:

    
signals:
    void commandComplite(TCPCommand*);
    void commandNotComplite(TCPCommand*);


    // ITCPCommand interface
public:
    virtual bool isCommandSet();
};

#endif /* TCPCOMMAND_H */

