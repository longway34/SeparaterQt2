#ifndef TCPGETSPECTRUMSGISTORGAMMS_H
#define TCPGETSPECTRUMSGISTORGAMMS_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "_types.h"

class TCPGetSpectrumsGistogramms : public TCPCommandSet
{
    Q_OBJECT
    
    uint threadNum;
    EnumCommands dataType;
    uint tTimeOut;

public:
    TCPGetSpectrumsGistogramms();
    TCPGetSpectrumsGistogramms(ServerConnect *_server, EnumCommands _dataType, TCPTimeOutWigget *_widget = nullptr, uint _threads = MAX_SPR_MAIN_THREADS);

    QByteArray getKSpectrumData(int thread = -1){
        QVector<TCPCommand*>vspect = findCommands(getkspk);
        QByteArray res0, res;
        if(vspect.size() > 0){
            if(thread < 0 || thread >= vspect.size())
                thread = vspect.size()-1;

            res0 = vspect[thread]->getReplayData();
//            res = res0.right(res0.size() - (5));
            res = res0.right(DEF_SPECTRUM_DATA_LENGTH_BYTE);
        }
        return res;
    }
    uint32_t getKSpectrumTime(int thread){
        QVector<TCPCommand*>vspect = findCommands(getkspk);
        QByteArray res0; uint32_t res = 0;
        if(vspect.size() > 0){
            if(thread < 0 || thread >= vspect.size())
                thread = vspect.size()-1;

            res0 = vspect[thread]->getReplayData();
            res0 = res0.left(5);
            res0 = res0.right(4);
            memcpy(&res, res0.constData(), sizeof(res));
//            res = res0.toInt();
        }
        return res;
    }
    QByteArray getSpectrumData(int thread){
        QVector<TCPCommand*>vspect = findCommands(getspk);
        QByteArray res;
        if(vspect.size() > 0){
            if(thread < 0 || thread >= vspect.size())
                thread = vspect.size()-1;

            res = vspect[thread]->getReplayData().right(DEF_SPECTRUM_DATA_LENGTH_BYTE);
        }
        return res;
    }

    QByteArray getGistogrammData(int thread){
        QVector<TCPCommand*> vcomm = findCommands(getgist);
        QByteArray res;
        if(vcomm.size() > 0){
            if(thread < 0 || thread >= vcomm.size())
                thread = vcomm.size()-1;

            res = vcomm[thread]->getReplayData();
        }
        return res;
    }

    void setThreadTimer(const uint &value, uint _time_in_sec = 1);

    virtual EnumCommands getDataType() const;

protected slots:
    virtual void go(TCPCommand *_command = NULL);
};

#endif // TCPGETSPECTRUMSGISTORGAMMS_H
