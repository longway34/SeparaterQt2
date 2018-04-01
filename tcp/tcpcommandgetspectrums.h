#ifndef TCPCOMMANDGETSPECTRUMS_H
#define TCPCOMMANDGETSPECTRUMS_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "models/sprmainmodel.h"
#include "models/sprgrspectrumitemmodel.h"


class TCPCommandGetSpectrums: public TCPCommandSet
{
    SPRMainModel *model;
    int timeOfSpectrum; // in seconds
    uint8_t channels;
    uint8_t channelCount;

    QVector<QByteArray> spectrumsData;

public:
    TCPCommandGetSpectrums();
    TCPCommandGetSpectrums(ServerConnect *_server, TCPTimeOutWigget *_widget, SPRMainModel *_model = nullptr, int _time = -1);

    // TCPCommand interface
    int getTimeOfSpectrum() const;
    void setTimeOfSpectrum(int value);

    QByteArray getSpectrum(uint _channel){
        QByteArray res;
        if(_channel < getChannels()){
            return spectrumsData[_channel];
        } else {
            return res;
        }
    }

    uint8_t getChannels() const;
    virtual QByteArray getResult()
    {
        return findCommands(getren).last()->getResult();
    }
    virtual QByteArray getReplayData()
    {
        return findCommands(getren).last()->getReplayData();
    }


    void setModel(SPRMainModel *value);

public slots:
    virtual void go(TCPCommand *_command);

};


#endif // TCPCOMMANDGETSPECTRUMS_H
