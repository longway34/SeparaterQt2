#include "tcpautosetcommandgo.h"

SPRThreadList TCPAutosetCommandGo::getThreads() const
{
    return threads;
}

void TCPAutosetCommandGo::setThreads(const SPRThreadList &value)
{
    threads = value;
}

TCPAutosetCommandGo::TCPAutosetCommandGo(ServerConnect *_server, QByteArray _deu, QByteArray _cp, SPRThreadList _threads, TCPTimeOutWigget *_toWidget, TCPLogsWigtets *_logs):
    TCPCommandSet(_server, _toWidget, {})
{
    setThreads(_threads);
    setDeuCpCodes(_deu, _cp);
    toSpk = 0x64;
}

void TCPAutosetCommandGo::setDeuCpCodes(QByteArray _deu, QByteArray _cp)
{
    baDEU = QByteArray(_deu); baCP = QByteArray(_cp);
}


void TCPAutosetCommandGo::go(TCPCommand *_command)
{
    if(!_command){
        clear();

        if(baDEU.size() > 0){
            addCommand(setudeu)->setSendData(baDEU);
            addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 100, getTimeOutWidget(),
                            MSG_TIME_OUT_SET_DEU, MSG_TIME_OUT_SET_DEU_MSG( 2 )));
//                          tr("Включение рентгена."), QString(tr("Установка кодов ДЭУ. (%1 сек.)")).arg(_time_out_on_deu_cp / 1000)));
        }
        if(baCP.size() > 0){
            addCommand(setptdeu)->setSendData(baCP);
            addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 100, getTimeOutWidget(),
                            MSG_TIME_OUT_SET_CP, MSG_TIME_OUT_SET_CP_MSG( 2 )));
//                            tr("Включение рентгена."), QString(tr("Установка кодов СР. (%1 сек.)")).arg(_time_out_on_deu_cp / 1000)));
        }

        addCommand(setspk)->setSendData(&toSpk, sizeof(toSpk));
        addCommand(new TCPTimeOutCommand(timeoutcommand, toSpk * 100 + 1000, 100, getTimeOutWidget(),
                        MSG_TIME_OUT_SET_SPK, MSG_TIME_OUT_SET_SPK_MSG( 10 )));
//                            tr("Накопление спектра"), QString(tr("Накопление спектра  (%1 сек.)")).arg(_time_out_spk / 1000)));

        foreach(uint8_t th, threads){
            addCommand(getspk)->setSendData(&th, sizeof(th));
        }
    }
    TCPCommandSet::go(_command);
}
