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

public:
    TCPGetSpectrumsGistogramms();
    TCPGetSpectrumsGistogramms(ServerConnect *_server, EnumCommands _dataType, uint _threads = MAX_SPR_MAIN_THREADS);

    QByteArray getKSpectrumData(int thread = -1){
        QVector<TCPCommand*>vspect = findCommands(getkspk);
        QByteArray res0, res;
        if(vspect.size() > 0){
            if(thread < 0 || thread >= vspect.size())
                thread = vspect.size()-1;

            res0 = vspect[thread]->getReplayData();
            res = res0.right(res0.size() - (5));
        }
        return res;
    }
    int getKSpectrumTime(int thread){
        QVector<TCPCommand*>vspect = findCommands(getkspk);
        QByteArray res0; int res = 0;
        if(vspect.size() > 0){
            if(thread < 0 || thread >= vspect.size())
                thread = vspect.size()-1;

            res0 = vspect[thread]->getReplayData();
            res0 = res0.right(5);
            res = res0.left(sizeof(int)).toUInt();
        }
        return res;
    }
    QByteArray getSpectrumData(int thread){
        QVector<TCPCommand*>vspect = findCommands(getspk);
        QByteArray res;
        if(vspect.size() > 0){
            if(thread < 0 || thread >= vspect.size())
                thread = vspect.size()-1;

            res = vspect[thread]->getReplayData();
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

    void setThreadNum(const uint &value);

protected slots:
    virtual void go(TCPCommand *_command = NULL);
};

#endif // TCPGETSPECTRUMSGISTORGAMMS_H
