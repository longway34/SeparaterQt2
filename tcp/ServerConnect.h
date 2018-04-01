/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ServerConnect.h
 * Author: longway
 *
 * Created on 9 февраля 2018 г., 17:06
 */

#ifndef SERVERCONNECT_H
#define SERVERCONNECT_H

#include <QTcpSocket>
#include <QQueue>
#include <QByteArray>
#include <QMutex>
#include "ITCPCommand.h"
//#include "TCPCommand.h"
#include "QTimer"

#define REMOTE_WAIT_C   0
#define REMOTE_SPK      1
#define REMOTE_SEP      2
#define REMOTE_RENT     3
#define REMOTE_TERM     4
#define REMOTE_TEST     5
#define REMOTE_RGU      6

#define SPR_STATE_WAIT_COMMAND          0
#define SPR_STATE_SPECTOR_SCOPE         0x00000001
#define SPR_STATE_SEPARATED             0x00000002
#define SPR_STATE_RENTGEN_UNKNOW        0x00000004
#define SPR_STATE_THERM_SET             0x00000008
#define SPR_STATE_TEST_IMS              0x00000010
#define SPR_STATE_RGU_MOVED             0x00000020

#define SPR_STATE_SEPATOR_ON            0x00010000
#define SPR_STATE_RENTGEN_ON            0x00020000
#define SPR_STATE_PITATEL_ON            0x00040000

#define SPR_STATE_RENTGEN_NOT_REGIME    0x02000000
#define SPR_STATE_ERROR_CONNECT         0x01000000

typedef enum server_connect_state :uint32_t {
    spr_state_wait_command          = SPR_STATE_WAIT_COMMAND,
    spr_state_spector_scope         = SPR_STATE_SPECTOR_SCOPE,
    spr_state_separated             = SPR_STATE_SEPARATED,
    spr_state_rentgen_uknown        = SPR_STATE_RENTGEN_UNKNOW,
    spr_state_therm_set             = SPR_STATE_THERM_SET,
    spr_state_test_ims              = SPR_STATE_TEST_IMS,
    spr_state_gru_moved             = SPR_STATE_RGU_MOVED,

    spr_state_separator_on          = SPR_STATE_SEPATOR_ON,
    spr_state_rentgen_on            = SPR_STATE_RENTGEN_ON,
    spr_state_pitatel_on            = SPR_STATE_PITATEL_ON,

    spr_state_rentgen_not_regime    = SPR_STATE_RENTGEN_NOT_REGIME,
    spr_state_error_connect         = SPR_STATE_ERROR_CONNECT

} ServerConnectState;


//#define SPR_STATE_

class ServerConnect: public QTcpSocket{

    Q_OBJECT
    
    class getCommand :public ITCPCommand{


        // ITCPCommand interface
    public:
        getCommand(EnumCommands _command){
            command = _command;
        }

        virtual void send(ServerConnect *server){
            server->addSendCommand(this);
        }

        virtual void setReplayData(QByteArray replayData){}
        virtual QByteArray getReplayData(){QByteArray res; return res;}
        virtual void setSendData(QByteArray sendData){}
        virtual QByteArray getSendData(){QByteArray res; return res;}
    };


    QString name;
    uint16_t port;
    QQueue<ITCPCommand*> sendData;
    QMutex mutex;
    
    ITCPCommand *current;
    getCommand *getStateCommand;
    QByteArray replay;
    QTimer toGetStateCommand;

    uint32_t currentState;
    uint32_t setState(uint32_t _state){
        bool flag = currentState != _state;
        currentState = _state;
        if(flag){
            emit serverStateChange(currentState);
        }
    }

    void changeRemoteState(QByteArray replay){
        if(replay.size() > 1){
            int _st = replay.at(1);
            uint32_t st;
            switch (_st) {
            case REMOTE_WAIT_C:
                st = currentState & 0xFFFFFF00;
                break;
            case REMOTE_SPK:
                st = (currentState & 0xFFFFFF00) | spr_state_spector_scope;
                break;
            case REMOTE_SEP:
                st = (currentState & 0xFFFFFF00) | spr_state_separated;
                break;
            case REMOTE_RENT:
                st = (currentState & 0xFFFFFF00) | spr_state_rentgen_uknown;
                break;
            case REMOTE_TERM:
                st = (currentState & 0xFFFFFF00) | spr_state_therm_set;
                break;
            case REMOTE_TEST:
                st = (currentState & 0xFFFFFF00) | spr_state_test_ims;
                break;
            case REMOTE_RGU:
                st = (currentState & 0xFFFFFF00) | spr_state_gru_moved;
                break;
            default:
                break;
            }
            setState(st);
        }
    }
public:
    ServerConnect(QString _name, uint16_t _port): QTcpSocket(){
        name = _name;
        port = _port;
        currentState = 0;
//        connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(this, SIGNAL(addData()), this, SLOT(queueComplite()));
        getStateCommand = new getCommand(getstate);
        toGetStateCommand.setInterval(10000);
        connect(&toGetStateCommand, SIGNAL(timeout()), this, SLOT(onTimeOutGetState()));
        toGetStateCommand.start();

    }
    virtual ~ServerConnect();
    
    void addSendCommand(ITCPCommand* _sender);
    void setPort(uint16_t port);
    uint16_t getPort() const;
    void setName(QString name);
    QString getName() const;

    uint32_t getState(){ return currentState;}
    uint32_t addState(ServerConnectState _state){
        if(_state > 0xFFFF){
            uint32_t newState = currentState | _state;
            setState(newState);
        }
        return getState();
    }
    uint32_t clearState(ServerConnectState _state){
        setState(currentState & (~((uint32_t)_state)));
        return getState();
    }

    bool isState(ServerConnectState _state){
        if(currentState & ((uint32_t)_state)){
            return true;
        } else {
            return false;
        }
    }

    void clearQueue();
public slots:
    void queueComplite();

protected slots:    
    void onReadyRead();
    void onTimeOutGetState(){
        getStateCommand->send(this);
    }

signals:
    void addData();
    void commandComlite(ITCPCommand*);
    void serverConnectTimeOutError(ITCPCommand*);
    void serverReadWriteTimeOutError(ITCPCommand*);
    void serverStateChange(uint32_t);

private:

};

#endif /* SERVERCONNECT_H */

