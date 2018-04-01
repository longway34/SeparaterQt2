/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TCPCommanSet.h
 * Author: longway
 *
 * Created on 10 февраля 2018 г., 18:32
 */

#ifndef TCPCOMMANSET_H
#define TCPCOMMANSET_H

#include "tcp/TCPTimeOutCommand.h"
#include <QVector>

class TCPCommandSet : public TCPTimeOutCommand{
protected:
    QVector<TCPCommand*> commandSet;
    
    ServerConnect *server;
    
public:
    TCPCommandSet();
    TCPCommandSet(TCPTimeOutWigget *_widget): TCPTimeOutCommand(nocommand, 0, 1, _widget, "", ""){}
    TCPCommandSet(ServerConnect *_server, TCPTimeOutWigget *_widget, QVector<TCPCommand*> _vcomm): TCPTimeOutCommand(nocommand, 0, 1, _widget, "", ""){
        server = _server;
        addCommand(_vcomm);
    }
    virtual ~TCPCommandSet();
    
    TCPCommandSet *addCommand(TCPCommand* _comm){
        _comm->setNum(commandSet.size());
        commandSet.push_back(_comm);
        connect(_comm, SIGNAL(commandComplite(TCPCommand*)), this, SLOT(go(TCPCommand*)));
        return this;
    }
    TCPCommandSet *addCommand(QVector<TCPCommand*> _vcomm){
        for(int i=0; i<_vcomm.size(); i++){
            addCommand(_vcomm[i]);
        }
        return this;
    }
    
    virtual void send(ServerConnect *_server){
        server = _server;
        go();
    }
    virtual int getErrors(){
        int ret = 0;
        for(int i=0; i<commandSet.size(); i++){
            ret |= commandSet[i]->getErrors();
        }
        return ret;
    }

    virtual int getState(){
        if(commandSet.size() > 0){
            return commandSet.last()->getState();
        } else {
            return nocommand;
        }
    }

    virtual int getState(EnumCommands _comm){
        if(_comm == lastcommand){
            return getState();
        } else {
            QVector<TCPCommand*> _vcomm = findCommands(_comm);
            if(_vcomm.size() > 0){
                return _vcomm[_vcomm.size()-1]->getState();
            } else {
                return getState();
            }
        }
    }
    virtual QByteArray getReplayData(){
        return this->getReplayData(lastcommand);
    }
    virtual QByteArray getReplayData(EnumCommands _comm){
        QByteArray ret;
        if(_comm == lastcommand){
            if(commandSet.size() > 0){
                ret = commandSet.last()->getReplayData();
            } else {
                return ret;
            }
        } else {
            QVector<TCPCommand *> _vcomm = findCommands(_comm);
            if(_vcomm.size() > 0){
                ret = _vcomm.last()->getReplayData();
            } else {
                return ret;
            }
        }
    }
    virtual EnumCommands getCommand(){
        if(commandSet.size() > 0){
            return commandSet.last()->getCommand();
        } else {
            return nocommand;
        }
    }
    virtual QByteArray getResult(){
        QByteArray ret;
        if(commandSet.size() > 0){
            return commandSet.last()->getResult();
        } else {
            return ret;
        }
    }
    virtual QByteArray getResult(EnumCommands _comm){
        QByteArray ret;
        QVector<TCPCommand *> _vcomm = findCommands(_comm);
        if(_vcomm.size()>0){
            ret = _vcomm[_vcomm.size()-1]->getResult();
        }
        return ret;
    }
    int findCommand(TCPCommand *_command){
        int res = -1;
        for(int i=0; i< commandSet.size(); i++){
            if(commandSet[i] == _command){
                res = _command->getNum();
            }
        }
        return res;
    }
    QVector<TCPCommand*> findCommands(EnumCommands _command=lastcommand){
        if(_command == lastcommand){
            return QVector<TCPCommand*>({commandSet.last()});
        } else {
            QVector<TCPCommand*> res;
            for(int i=0; i<commandSet.size();i++){
                TCPCommand *comm = commandSet[i];
                if(comm->getCommand() == _command){
                    res.push_back(comm);
                }
            }
            return res;
        }
    }
protected:
//    virtual bool isCondition();
    
protected slots:
    virtual void go(TCPCommand *_command = NULL);
};

#endif /* TCPCOMMANSET_H */

