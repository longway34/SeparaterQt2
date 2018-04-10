#ifndef TCPGETRENTGENSPARAMS_H
#define TCPGETRENTGENSPARAMS_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "_types.h"

class TCPGetRentgensParams : public TCPCommandSet
{
    uint rentgens;
public:
    TCPGetRentgensParams();
    TCPGetRentgensParams(ServerConnect *_server) :
        TCPCommandSet(_server, nullptr, {}), rentgens(DEF_SPR_MAIN_RENTGENS)
    {
        command = setGetRentgenParams;
    }

    uint getVARentgen(bool VA, int num = -1){
        uint V = 0, A=0;
        if(commandSet.size() > 0){
            QByteArray res0;
            if(num < 0 && num >= commandSet.size()){
                num = 0;
            }
            res0 = commandSet[num]->getReplayData();
            res0 = res0.right(res0.size() - 1);
            A = res0.right(2).toInt();
            V = res0.left(2).toInt();
        }
        if(VA){
            return V;
        } else {
            return A;
        }
    }

    uint getRentgens() const;
    void setRentgens(const uint &value);
};

#endif // TCPGETRENTGENSPARAMS_H
