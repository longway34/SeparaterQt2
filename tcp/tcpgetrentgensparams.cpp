#include "tcpgetrentgensparams.h"

uint TCPGetRentgensParams::getRentgens() const
{
    return rentgens;
}

void TCPGetRentgensParams::setRentgens(const uint &value)
{
    rentgens = value;
    for(int i=0; i<commandSet.size(); i++) delete commandSet[i];
    commandSet.clear();

    for(uint8_t i=0; i<rentgens; i++){
        TCPCommand *gr = new TCPCommand(getren);
        gr->setReplayData(QByteArray((char*)&i, sizeof(i)));
        addCommand(gr);
    }
}

TCPGetRentgensParams::TCPGetRentgensParams()
{

}
