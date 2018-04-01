#ifndef TCPSTARTSOPTESTIMSCOMMAND_H
#define TCPSTARTSOPTESTIMSCOMMAND_H

#include <QObject>
#include "tcp/TCPCommandSet.h"

class TCPStartSopTestIMSCommand : public TCPCommandSet
{
    QVector<uint8_t> ims;
    uint16_t delay; // msec.
    uint16_t timeWork; // sec.
    uint16_t freq; // Gz
    int countIms;

    TCPCommand *stopTestCommand;
public:
    TCPStartSopTestIMSCommand();
    TCPStartSopTestIMSCommand(ServerConnect *_server, TCPTimeOutWigget *_widget);

    void setParams(QVector<uint8_t> _ims, int _delay, int _timeWork, int _freq){
        ims = _ims;
        delay = _delay;
        timeWork = _timeWork;
        freq = _freq;
//        findCommands(timeoutcommand).first()->setTimeOut(timeWork);
    }

    // TCPCommand interface
    virtual QByteArray getReplayData();
    virtual QByteArray getResult();
    TCPCommand *getStopTestCommand() const;

public slots:
    virtual void go(TCPCommand *_command);
    void stopTest();
};



#endif // TCPSTARTSOPTESTIMSCOMMAND_H
