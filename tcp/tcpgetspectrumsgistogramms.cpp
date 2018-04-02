#include "tcpgetspectrumsgistogramms.h"

void TCPGetSpectrumsGistogramms::setThreadNum(const uint &value)
{
    threadNum = value;
    for(int i=0; i< commandSet.size(); i++){
        delete commandSet[i];
    }
       commandSet.clear();
       for(uint8_t i=0; i<threadNum; i++){
           TCPCommand *comm = new TCPCommand(command);
           comm->setReplayData(QByteArray::fromRawData((char*)&i,sizeof(i)));
           addCommand(comm);
       }

}

TCPGetSpectrumsGistogramms::TCPGetSpectrumsGistogramms()
{
}

TCPGetSpectrumsGistogramms::TCPGetSpectrumsGistogramms(ServerConnect *_server, EnumCommands _dataType, uint _threads):
    TCPCommandSet(_server, nullptr, {}), threadNum(_threads), dataType(_dataType)
{
}

void TCPGetSpectrumsGistogramms::go(TCPCommand *_command)
{
    TCPCommandSet::go(_command);
}

