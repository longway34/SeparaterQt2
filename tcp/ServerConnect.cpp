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

TCPLogsWigtets *ServerConnect::getLogWidget() const
{
    return logWidget;
}

void ServerConnect::setLogWidget(TCPLogsWigtets *value)
{
    logWidget = value;
}

SPRVariable<uint> *ServerConnect::getVPort() const
{
    return vPort;
}

void ServerConnect::setVPort(SPRVariable<uint> *value)
{
    vPort = value;
    if(vPort){
        port = vPort->getData();
    }
}

SPRQStringVariable *ServerConnect::getVName() const
{
    return vName;
}

void ServerConnect::setVName(SPRQStringVariable *value)
{
    vName = value;
    if(vName){
        name = vName->getData();
    }
}

void ServerConnect::setError(ITCPCommand *command){
    QString msg;
    int err = command->getErrors();
    if(err < 0){
        if(logWidget){
            msg = QString(tr("Ошибка выполнения команды. Нет ответа..."));
            logWidget->onErrorLogsCommand(command, msg);
        }
        return;
    }
    lastCommandError = static_cast<ServerCommandState>(uint(command->getErrors()));
    if(lastCommandError != spr_remote_error_ok){
        if(logWidget){
            switch(lastCommandError){
            case spr_remote_error_unknown_command:
                msg = QString(tr("Ошибка! Неизвестная команда..."));
                break;
            case spr_remote_error_no_data:
                msg = QString(tr("Ошибка! Нехватает данных..."));
                break;
            case spr_remote_error_no_correct_data:
                msg = QString(tr("Ошибка! Некорректные данные..."));
                break;
            case spr_remote_error_not_ready_get_command:
            case spr_remote_error_not_ready_hardware:
                msg = QString(tr("Ошибка! Несвоевременная подача команды..."));
                break;
            case spr_remote_error_er_ser:
                msg = QString(tr("Ошибка! ER_SER..."));
                break;
            case spr_remote_error_er_rent:
                msg = QString(tr("Ошибка! ER_RENT..."));
                break;
            case spr_remote_error_er_term:
                msg = QString(tr("Ошибка! ER_TERM..."));
                break;
            case spr_remote_error_er_im:
                msg = QString(tr("Ошибка! ER_IM..."));
                break;
            }
            logWidget->onErrorLogsCommand(command, msg);
        }
    }
}

void ServerConnect::changeRemoteState(QByteArray replay){
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

ServerConnect::~ServerConnect() {
}

void ServerConnect::setPort(uint16_t port) {
    this->port = port;
}

uint16_t ServerConnect::getPort() const {
    if(vPort){
        return vPort->getData();
    } else {
        return port;
    }
}

void ServerConnect::setName(QString name) {
    this->name = name;
}

QString ServerConnect::getName() const {
    if(vName){
        return vName->getData();
    } else {
        return name;
    }
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
            connectToHost(getName(), getPort());
            if(!waitForConnected(500)){
                mutex.unlock();
                emit serverConnectTimeOutError(current);
               clearState(spr_state_server_connect);
               addState(spr_state_error_connect);
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
            clearState(spr_state_server_connect);
            addState(spr_state_error_connect);
        }
   } 
}

void ServerConnect::onReadyRead(){
    replay.clear();
    while(bytesAvailable()){
        replay.append(readAll());
    }
    mutex.unlock();
    clearState(spr_state_error_connect);
    addState(spr_state_server_connect);
    if(current->getCommand() == getstate){
        changeRemoteState(replay);
    }
    current->setReplayData(replay);
    setError(current);
    //    emit commandComlite(current);
}

void ServerConnect::onServerStateChange(uint32_t _state)
{

    if(logWidget){
        uint32_t bits = currentState ^ _state;
        uint32_t onBits = _state & bits;
        uint32_t offBits = currentState & bits;

        if((onBits & SPR_STATE_SERVER_CONNECT) == SPR_STATE_SERVER_CONNECT){
            logWidget->onLogsCommand(nullptr, QString(tr("Установлено соединение с сервером %1:%2")).arg(getName()).arg(getPort()));
        }
        if((onBits & SPR_STATE_ERROR_CONNECT) == SPR_STATE_ERROR_CONNECT){
            logWidget->onErrorLogsCommand(nullptr, QString("Нет соединения с сервером ")+getName()+":"+QString::number(getPort()));
        }
        if((onBits & SPR_STATE_RENTGEN_ON) == SPR_STATE_RENTGEN_ON){
            logWidget->onLogsCommand(nullptr, QString(tr("Рентген включен...")));
        }
        if((offBits & SPR_STATE_RENTGEN_ON) == SPR_STATE_RENTGEN_ON){
            logWidget->onLogsCommand(nullptr, QString(tr("Рентген выключен...")));
        }
        if((onBits & SPR_STATE_SEPATOR_ON) == SPR_STATE_SEPATOR_ON){
            logWidget->onLogsCommand(nullptr, QString(tr("Сепаратор включен...")));
        }
        if((offBits & SPR_STATE_SEPATOR_ON) == SPR_STATE_SEPATOR_ON){
            logWidget->onLogsCommand(nullptr, QString(tr("Сепаратор выключен...")));
        }
        if((onBits & SPR_STATE_RGU_MOVED) == SPR_STATE_RGU_MOVED){
            logWidget->onLogsCommand(nullptr, QString(tr("Движение РГУ...")));
        }
        if((offBits & SPR_STATE_RGU_MOVED) == SPR_STATE_RGU_MOVED){
            logWidget->onLogsCommand(nullptr, QString(tr("Движение РГУ завершено...")));
        }
        if((onBits & SPR_STATE_TEST_IMS) == SPR_STATE_TEST_IMS){
            logWidget->onLogsCommand(nullptr, QString(tr("Тест исполнительных механизмов начат...")));
        }
        if((offBits & SPR_STATE_TEST_IMS) == SPR_STATE_TEST_IMS){
            logWidget->onLogsCommand(nullptr, QString(tr("Тест исполнительных механизмов завершен...")));
        }
        if((onBits & SPR_STATE_SPECTOR_SCOPE) == SPR_STATE_SPECTOR_SCOPE){
            logWidget->onLogsCommand(nullptr, QString(tr("Режим накопления спектра...")));
        }
        if((offBits & SPR_STATE_SPECTOR_SCOPE) == SPR_STATE_SPECTOR_SCOPE){
            logWidget->onLogsCommand(nullptr, QString(tr("Режим накопления спектра завершен...")));
        }
        if((onBits & SPR_STATE_PITATEL_ON) == SPR_STATE_PITATEL_ON){
            logWidget->onLogsCommand(nullptr, QString(tr("Питатель включен...")));
        }
        if((offBits & SPR_STATE_PITATEL_ON) == SPR_STATE_PITATEL_ON){
            logWidget->onLogsCommand(nullptr, QString(tr("Питатель выключен...")));
        }
        if((onBits & SPR_STATE_SEPARATED) == SPR_STATE_SEPARATED){
            logWidget->onLogsCommand(nullptr, QString(tr("Сепарация началась...")));
        }
        if((offBits & SPR_STATE_SEPARATED) == SPR_STATE_SEPARATED){
            logWidget->onLogsCommand(nullptr, QString(tr("Сепарация закончилась...")));
        }
        if((onBits & SPR_STATE_EXPOSITION_ON) == SPR_STATE_EXPOSITION_ON){
            logWidget->onLogsCommand(nullptr, QString(tr("Экспозиция включена...")));
        }
        if((offBits & SPR_STATE_EXPOSITION_ON) == SPR_STATE_EXPOSITION_ON){
            logWidget->onLogsCommand(nullptr, QString(tr("Экспозиция выключена...")));
        }
    }
    currentState = _state;
}



