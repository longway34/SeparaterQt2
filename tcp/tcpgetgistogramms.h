#ifndef TCPGETGISTOGRAMMS_H
#define TCPGETGISTOGRAMMS_H

#include <QObject>
#include "tcp/TCPCommandSet.h"

class TCPGetGistogramms : public TCPCommandSet
{
    Q_OBJECT
    
    uint threadNum;

public:
    TCPGetGistogramms();
    TCPGetGistogramms(TCPTimeOutWigget *_widget, uint _threads);

    QByteArray getGistorrammData(int thread){
        QVector<TCPCommand*>vspect = findCommands(getkspk);
        QByteArray res;
        if(vspect.size() > 0){
            if(thread < 0 || thread >= vspect.size())
                thread = vspect.size()-1;

            res = vspect[thread]->getReplayData();
        }
        return res;
    }
protected slots:
//    virtual void go(TCPCommand *_command = NULL);
};

#endif // TCPGETGISTOGRAMMS_H
