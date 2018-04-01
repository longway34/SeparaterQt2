#include "tcpstartseparate.h"

TCPStartSeparate::TCPStartSeparate()
{
    
}

TCPStartSeparate::TCPStartSeparate(ServerConnect *_server, TCPTimeOutWigget *_widget)
{
    
}

void TCPStartSeparate::go(TCPCommand *_command)
{
    addCommand(new TCPCommand(getrgu2));
    addCommand(new TCPCommand(getblk));
    addCommand(new TCPCommand(expon));
    uint8_t rentgenNum = 0;
    commandSet.at(commandSet.size()-1)->setSendData(QByteArray::fromRawData((char*)&rentgenNum, sizeof(rentgenNum)));
    
    addCommand(new TCPCommand(offosw));
    addCommand(new TCPCommand());
}
