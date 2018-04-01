#ifndef TCPGETKSPECTRUMS_H
#define TCPGETKSPECTRUMS_H

#include <QObject>
#include "tcp/TCPCommandSet.h"

class TCPGetKSpectrums : public TCPCommandSet
{
    Q_OBJECT
    
    uint threadNum;

public:
    TCPGetKSpectrums();
    TCPGetKSpectrums(TCPTimeOutWigget *_widget, uint _threads);

    QByteArray getSpectrumData(int num){
        QVector<TCPCommand*>vspect = findCommands(getkspk);
        QByteArray res0, res;
        if(vspect.size() > 0){
            if(num < vspect.size()){
                res0 = vspect[num]->getReplayData();
            } else {
                res0 = vspect.last()->getReplayData();
            }
            res = res0.left(res0.size() - (1 + sizeof(int)));
        }
        return res;
    }
    int getSpectrumTime(int num){

    }
protected slots:
//    virtual void go(TCPCommand *_command = NULL);
};

#endif // TCPGETKSPECTRUMS_H
