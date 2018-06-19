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
#include "tcp/tcplogswigtets.h"
#include "tcp/ITCPCommand.h"
#include "models/imodelvariable.h"
#include "variables/sprvariable.h"
#include "variables/sprqstringvariable.h"
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
#define SPR_STATE_EXPOSITION_ON         0x00080000

#define SPR_STATE_RENTGEN_NOT_REGIME    0x02000000
#define SPR_STATE_ERROR_CONNECT         0x01000000
#define SPR_STATE_SERVER_CONNECT        0x04000000

#define SPR_REMOTE_ERROR_OK                     0x0 // Ok
#define SPR_REMOTE_ERROR_UKNOWN_COMMAND         0x1 // N_KM
#define SPR_REMOTE_ERROR_NO_DATA                0x2 // N_D_KM
#define SPR_REMOTE_ERROR_NO_CORRECT_DATA        0x3 // N_DATE
#define SPR_REMOTE_ERROR_NOT_READY_GET_COMMAND  0x4 // N_EX
#define SPR_REMOTE_ERROR_NOT_READY_HARDWARE     0x5 // NN_EX
#define SPR_REMOTE_ERROR_ER_SER                 0x6 // ER_SER
#define SPR_REMOTE_ERROR_ER_RENT                0x7 // ER_RENT
#define SPR_REMOTE_ERROR_ER_TERM                0x8 // ER_TERM
#define SPR_REMOTE_ERROR_ER_IM                  0x9 // ER_IM
#define SPR_REMOTE_ERROR_TCP                    (-1)

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
    spr_state_exposition_on         = SPR_STATE_EXPOSITION_ON,

    spr_state_rentgen_not_regime    = SPR_STATE_RENTGEN_NOT_REGIME,
    spr_state_error_connect         = SPR_STATE_ERROR_CONNECT,
    spr_state_server_connect        = SPR_STATE_SERVER_CONNECT

} ServerConnectState;

typedef enum server_command_state :uint{
    spr_remote_error_ok                     = SPR_REMOTE_ERROR_OK,
    spr_remote_error_unknown_command        = SPR_REMOTE_ERROR_UKNOWN_COMMAND,
    spr_remote_error_no_data                = SPR_REMOTE_ERROR_NO_DATA,
    spr_remote_error_no_correct_data        = SPR_REMOTE_ERROR_NO_CORRECT_DATA,
    spr_remote_error_not_ready_get_command  = SPR_REMOTE_ERROR_NOT_READY_GET_COMMAND,
    spr_remote_error_not_ready_hardware     = SPR_REMOTE_ERROR_NOT_READY_HARDWARE,
    spr_remote_error_er_ser                 = SPR_REMOTE_ERROR_ER_SER,
    spr_remote_error_er_rent                = SPR_REMOTE_ERROR_ER_RENT,
    spr_remote_error_er_term                = SPR_REMOTE_ERROR_ER_TERM,
    spr_remote_error_er_im                  = SPR_REMOTE_ERROR_ER_IM
} ServerCommandState;

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

        virtual void setReplayData(QByteArray replayData){this->replayData = replayData;}
        virtual QByteArray getReplayData(){return replayData;}
        virtual void setSendData(QByteArray sendData){}
        virtual QByteArray getSendData(){return sendData;}

        // ITCPCommand interface
    public:
        virtual int getErrors()
        {
            if(replayData.size()>0){
                return int(replayData[0]);
            }
            return -1;
        }

        // ITCPCommand interface
    public:
        virtual bool noErrors()
        {
            int res = getErrors();
            if(res < 0 || res != 0){
                return false;
            } else {
                return true;
            }

        }
    };


    QQueue<ITCPCommand*> sendData;
    QMutex mutex;
    
    ITCPCommand *current;
    getCommand *getStateCommand;
    QByteArray replay;
    QTimer timerGetStateCommand;

    TCPLogsWigtets *logWidget;

    ServerCommandState lastCommandError;

    uint32_t currentState;
    uint32_t setState(uint32_t _state){
        bool flag = currentState != _state;
//        currentState = _state;
        if(flag){
            emit serverStateChange(_state);
        }
    }

protected:
    QString name;
    uint16_t port;
    SPRVariable<uint> *vPort;
    SPRQStringVariable *vName;

    void setError(ITCPCommand *command);

    void changeRemoteState(QByteArray replay);
public:
    ServerConnect(QString _name, uint16_t _port, SPRQStringVariable *_vName = nullptr, SPRVariable<uint> *_vPort = nullptr): QTcpSocket(){
        name = _name;
        port = _port;
        setVName(_vName);
        setVPort(_vPort);

        currentState = 0;

        connect(this, SIGNAL(serverStateChange(uint32_t)), this, SLOT(onServerStateChange(uint32_t)));
//        connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(this, SIGNAL(addData()), this, SLOT(queueComplite()));
        getStateCommand = new getCommand(getstate);
//        timerGetStateCommand.setInterval(2000);
        connect(&timerGetStateCommand, SIGNAL(timeout()), this, SLOT(onTimeOutGetState()));
//        timerGetStateCommand.start();
        setTimer(fastTypeServerTimer);

    }
    virtual ~ServerConnect();
    
    void timerStart(){timerGetStateCommand.start();}
    void timerStop(){timerGetStateCommand.start();}

    typedef enum serverTimerType{
        fastTypeServerTimer, slowTypeServerTimer
    } ServerTimerType;


    void setTimer(ServerTimerType _type = fastTypeServerTimer){
        timerGetStateCommand.stop();
        if(_type == fastTypeServerTimer){
            timerGetStateCommand.setInterval(2000);
        } else if(_type == slowTypeServerTimer){
            timerGetStateCommand.setInterval(20000);
        }
        timerGetStateCommand.start();
    }

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

    void serverReconnect(){
        this->disconnectFromHost();
        clearState(spr_state_rentgen_on);
        clearState(spr_state_separator_on);
        clearState(spr_state_exposition_on);
    }
    bool isState(ServerConnectState _state){
        if(currentState & ((uint32_t)_state)){
            return true;
        } else {
            return false;
        }
    }

    void clearQueue();
    TCPLogsWigtets *getLogWidget() const;
    void setLogWidget(TCPLogsWigtets *value);

    SPRVariable<uint> *getVPort() const;
    void setVPort(SPRVariable<uint> *value);

    SPRQStringVariable *getVName() const;
    void setVName(SPRQStringVariable *value);

public slots:
    void queueComplite();
    void onChangeModelData(IModelVariable *var);

protected slots:    
    void onReadyRead();
    void onTimeOutGetState(){
        getStateCommand->send(this);
    }
    void onServerStateChange(uint32_t _state);

signals:
    void addData();
    void commandComlite(ITCPCommand*);
    void commandError(ITCPCommand*, ServerCommandState);
    void serverConnectTimeOutError(ITCPCommand*);
    void serverReadWriteTimeOutError(ITCPCommand*);
    void serverStateChange(uint32_t);


};

#endif /* SERVERCONNECT_H */

