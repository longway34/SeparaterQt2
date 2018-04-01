#include "tcpgetkspectrums.h"

TCPGetKSpectrums::TCPGetKSpectrums()
{

}

TCPGetKSpectrums::TCPGetKSpectrums(TCPTimeOutWigget *_widget, uint _threads):
    TCPCommandSet(_widget), threadNum(_threads)
{
    for(uint8_t i=0; i<threadNum; i++){
        TCPCommand *command = new TCPCommand(getkspk);
        command->setSendData(QByteArray::fromRawData((char*)&i, sizeof(i)));
        addCommand(command);
    }
}

//void TCPGetKSpectrums::go(TCPCommand *_command)
//{
//    if(!_command){
////        threadCount = 0;
//        commandSet[0]->send(server);
//    } else {
//        if(_command->getNum() < threadNum-1){
//            commandSet[_command->getNum()+1]->send(server);
//        } else {
//            emit commandComplite(this);
//        }
//    }
//}
