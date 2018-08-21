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

#define MSG_TIME_OUT_SET_DEU               (QString(tr("Включение рентгена")))
#define MSG_TIME_OUT_SET_DEU_MSG(X)        (QString(tr("Установка кодов ДЭУ...(%1)")).arg( (X) ))

#define MSG_TIME_OUT_SET_CP                (QString(tr("Включение рентгена")))
#define MSG_TIME_OUT_SET_CP_MSG(X)         (QString(tr("Установка кодов ЦП...(%1)")).arg( (X) ))

#define MSG_TIME_OUT_REN_ON                (QString(tr("Включение рентгена")))
#define MSG_TIME_OUT_REN_ON_MSG(X)         (QString(tr("Прогрев рентгена...(%1)")).arg( (X) ))

#define MSG_TIME_OUT_EXP_ON                (QString(tr("Включение рентгена")))
#define MSG_TIME_OUT_EXP_ON_MSG(X)         (QString(tr("Установка экспозиции ...(%1)")).arg( (X) ))

#define MSG_TIME_OUT_SET_SPK               (QString(tr("Накопление спектра")))
#define MSG_TIME_OUT_SET_SPK_MSG(X)        (QString(tr("Накопление спектра ...(%1)")).arg( (X) ))

#define MSG_TIME_OUT_ON_SEPAR               (QString(tr("Старт сепарации")))
#define MSG_TIME_OUT_ON_SEPAR_MSG(X)        (QString(tr("Старт сепарации...(%1)")).arg( (X) ))

#define MSG_TIME_OUT_ON_PITATEL               (QString(tr("Включение питателя")))
#define MSG_TIME_OUT_ON_PITAPEL_MSG(X)        (QString(tr("Включение питателя...(%1)")).arg( (X) ))

#define MSG_TIME_OUT_OFF_PITATEL               (QString(tr("Выключение питателя")))
#define MSG_TIME_OUT_OFF_PITAPEL_MSG(X)        (QString(tr("Выключение питателя...(%1)")).arg( (X) ))

#define MSG_TIME_OUT_ON_RASKLAD               (QString(tr("Включение раскладчика")))
#define MSG_TIME_OUT_ON_RASKLAD_MSG(X)        (QString(tr("Включение раскладчика...(%1)")).arg( (X) ))

#define MSG_TIME_OUT_OFF_SEPAR               (QString(tr("Останов сепарации")))
#define MSG_TIME_OUT_OFF_SEPAR_MSG(X)        (QString(tr("Останов сепарации...(%1)")).arg( (X) ))

#include "tcp/TCPTimeOutCommand.h"
#include <QVector>

class TCPCommandSet : public TCPTimeOutCommand{
protected:
//    QVector<TCPCommand*> commandSet;

    ServerConnect *server;
    
public:
    TCPCommandSet();
//    TCPCommandSet(TCPTimeOutWigget *_widget): TCPTimeOutCommand(nocommand, 0, 1, _widget, "", ""){}
    TCPCommandSet(ServerConnect *_server, TCPTimeOutWigget *_widget, QVector<TCPCommand*> _vcomm);
    TCPTimeOutWigget *getTimeOutWidget(){
        return widget;
    }
    virtual ~TCPCommandSet();
    
    TCPCommandSet *addCommand(TCPCommand* _comm);
    TCPCommandSet *addCommand(QVector<TCPCommand*> _vcomm);
    
    TCPCommandSet *addCommand(QVector<EnumCommands> _ecomm);

    TCPCommandSet *addCommand(EnumCommands ecommand);


    virtual void send(ServerConnect *_server);
    virtual int getErrors(){
        int ret = 0;
        for(int i=0; i<commandSet.size(); i++){
            ret |= commandSet[i]->getErrors();
        }
        return ret;
    }

    virtual TCPCommand *setSendData(QByteArray _sendData, EnumCommands _command = lastcommand);
    virtual TCPCommand *setSendData(void *data, int len, EnumCommands _command = lastcommand);

    virtual TCPCommand *addSendData(QByteArray sendData, EnumCommands _command = lastcommand);

    virtual TCPCommand *addSendData(void *data, uint len, EnumCommands _command = lastcommand);

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
        return ret;
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
    TCPCommand* findCommand(TCPCommand *_command);

    virtual bool isCommamdCompare(TCPCommand *_command){
        return true;
    }
//    QVector<TCPCommand*> findCommands(EnumCommands _command=lastcommand){
//        if(_command == lastcommand){
//            return QVector<TCPCommand*>({commandSet.last()});
//        } else {
//            QVector<TCPCommand*> res;
//            for(int i=0; i<commandSet.size();i++){
//                TCPCommand *comm = commandSet[i];
//                if(comm->getCommand() == _command){
//                    res.push_back(comm);
//                }
//            }
//            return res;
//        }
//    }
protected:
//    virtual bool isCondition();
    
protected slots:
    virtual void go(TCPCommand *_command = NULL);
    virtual void onCommandNotComplite(TCPCommand*_command);

    // ITCPCommand interface
public:
    virtual void setLogWidget(TCPLogsWigtets *value);

    // ITCPCommand interface
public:
    virtual bool noErrors();

    // ITCPCommand interface
public:
    virtual bool isCommandSet();
};

#endif /* TCPCOMMANSET_H */

