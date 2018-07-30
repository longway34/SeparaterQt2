/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ITCPCommand.h
 * Author: longway
 *
 * Created on 10 февраля 2018 г., 16:47
 */

#ifndef ITCPCOMMAND_H
#define ITCPCOMMAND_H
#include <QObject>
#include <QByteArray>
#include <QString>

//#include "tcp/tcplogswigtets.h"

typedef enum _tcp_commands: unsigned char{
            getstate    = 0x0A,
            testim      = 0x0B,
            initada     = 0x0C,
            getver      = 0x0D,
            stoptest    = 0x0F,
            setspk      = 0x10,
            getspk      = 0x11,
            setsepar    = 0x20,
            getsepar    = 0x21,
            startsep    = 0x22,
            stopsep     = 0x23,
            getkspk     = 0x24,
            getgist     = 0x25,
            startpuw    = 0x27,
            setpuw      = 0x28,
            stoppuw     = 0x29,
            setudeu     = 0x30,
            setren      = 0x31,
            getren      = 0x32,
            onren       = 0x33,
            offren      = 0x34,
            setrgu      = 0x35,
            settemp     = 0x37,
            gettemp     = 0x38,
            expon       = 0x40,
            expoff      = 0x41,
            onsep       = 0x50,
            offsep      = 0x51,
            oniw        = 0x52,
            offiw       = 0x53,
            onosw       = 0x54,
            offosw      = 0x55,
            setrgu2     = 0x56,
            getrgu2     = 0x57,
            setim       = 0x58,
            setptdeu    = 0x63,
            setm50      = 0x65,
            getm50      = 0x66,
            getblk      = 0x67,
            setlog      = 0x68,
                    
            lastcommand         = 0x7D,
            timeoutcommand      = 0x7E,
            nocommand           = 0x7F,

            setRentgenOn        = 0x80,
            setRGUUpDown        = 0x81,
            setStartPitatel     = 0x82,
            setStopPitatel      = 0x83,
            setGetSpectrumsGistorfamms  =0x84,
            setStartStopIMS    =0x85,
            setStartRudospusk   =0x86,
            setStartStopSeparate = 0x87,
            setTestStartSeparate = 0x88,
            setGetSpectrums      = 0x89,
            setRGUUp             = 0x8A,
            setSeparatorOn       = 0x8B,
            setStopRudospuk      = 0x8C,
            setGetRentgenParams  = 0x8D,
            setTestStopSeparate  = 0x8E,
            setSeparateGo        = 0x8F,
            setSeparatorOnFull     = 0x90

} EnumCommands;

typedef QMap<EnumCommands, QString> TCPPseudoName;

class TCPLogsWigtets;

class ITCPCommand :public QObject{
    
    Q_OBJECT
protected:
    EnumCommands command;
    int num;
    QString name;
    QByteArray sendData;
    QByteArray replayData;
    
    TCPLogsWigtets *logWidget;

public:
    ITCPCommand();
    virtual ~ITCPCommand();

    virtual QByteArray toQueue(){
        QByteArray ret;
        ret.append((char*)&command, sizeof(command)).append(sendData);
        return ret;
    }
    
    QString toString(QByteArray buf){
        QString rep;
        rep = QString(buf.toHex(':'));
        return rep;
    }
    
    void setNum(int num) {
        this->num = num;
    }

    int getNum() const {
        return num;
    }

    virtual void setCommand(EnumCommands command) {
        this->command = command;
    }

    virtual EnumCommands getCommand() {
        return command;
    }
    virtual void setReplayData(QByteArray replayData)=0;
    virtual QByteArray getReplayData() = 0;
//    virtual void addSendData(QByteArray sendData, EnumCommands)=0;
    virtual QByteArray getSendData()=0;
    virtual int getErrors()=0;
    virtual bool noErrors() = 0;
    virtual bool isCommandSet() = 0;
    virtual TCPLogsWigtets *getLogWidget() const;
    virtual void setLogWidget(TCPLogsWigtets *value);

    QString getName() const;
    void setName(const QString &value);

private:

};

#endif /* ITCPCOMMAND_H */

