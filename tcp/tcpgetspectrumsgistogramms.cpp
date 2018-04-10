#include "tcpgetspectrumsgistogramms.h"

void TCPGetSpectrumsGistogramms::setThreadTimer(const uint &value, uint _time_in_sec)
{
    threadNum = value; tTimeOut = _time_in_sec;
    for(int i=0; i< commandSet.size(); i++){
        delete commandSet[i];
    }
    commandSet.clear();

       int tTimer = 1;
       uint32_t tArg = tTimeOut * 10;
       if(dataType == getspk){
           TCPCommand *setRen = new TCPCommand(setspk);
//           QByteArray data;
//           data.append((char*)&tArg, sizeof(tArg));
           setRen->setSendData(&tArg, sizeof(tArg));
           addCommand(setRen);
           addCommand(new TCPTimeOutCommand(timeoutcommand, tTimeOut*1000+2000, tTimeOut, this->widget, tr("Накопление спектра"), tr("Накопление спектра")));
       }
       for(uint8_t i=0; i<threadNum; i++){
           TCPCommand *comm = new TCPCommand(dataType);
//           uint8_t arg = i;
//           QByteArray data;
//           data.append((char*)&arg, sizeof(arg));
           comm->setSendData(&i, sizeof(i));
           addCommand(comm);
       }

}

EnumCommands TCPGetSpectrumsGistogramms::getDataType() const
{
    return dataType;
}

TCPGetSpectrumsGistogramms::TCPGetSpectrumsGistogramms()
{
}

TCPGetSpectrumsGistogramms::TCPGetSpectrumsGistogramms(ServerConnect *_server, EnumCommands _dataType, TCPTimeOutWigget *_widget, uint _threads):
    TCPCommandSet(_server, _widget, {}), threadNum(_threads), dataType(_dataType)
{
    command = setGetSpectrumsGistorfamms;
    setThreadTimer(threadNum);
}

void TCPGetSpectrumsGistogramms::go(TCPCommand *_command)
{
//    if(!_command){
//        QVector<TCPCommand *>vcomm = findCommands(dataType);
//        for(uint8_t th=0; th<vcomm.size();th++){
//            TCPCommand *comm = vcomm[th];
//            uint8_t arg = th; int sarg = sizeof(arg);
//            QByteArray data;
//            data.insert(0, (char*)&arg, sarg);
//            comm->setSendData(data);
//        }
//    }
    TCPCommandSet::go(_command);
}

