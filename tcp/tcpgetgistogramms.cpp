#include "tcpgetgistogramms.h"

TCPGetGistogramms::TCPGetGistogramms()
{

}

TCPGetGistogramms::TCPGetGistogramms(TCPTimeOutWigget *_widget, uint _threads):
    TCPCommandSet(_widget), threadNum(_threads)
{
    for(uint8_t i=0; i<threadNum; i++){
        TCPCommand *command = new TCPCommand(getkspk);
        command->setSendData(QByteArray::fromRawData((char*)&i, sizeof(i)));
        addCommand(command);
    }
}

