#include "tcpgetspectrumsgistogramms.h"
#include "tcp/tcpexpositiononoff.h"
#include "tcp/tcpcommandrguupdown.h"

void TCPGetSpectrumsGistogramms::setThreadTimer(double _time_spk_in_sec, QList<uint8_t> _wtList)
{

    timeOfSpectorScope_in_sec = _time_spk_in_sec;
    workingThreads = _wtList;
}

EnumCommands TCPGetSpectrumsGistogramms::getDataType() const
{
    return dataType;
}

//TCPLogsWigtets *TCPGetSpectrumsGistogramms::getLogWidget() const
//{
//    return logWidget;
//}

//void TCPGetSpectrumsGistogramms::setLogWidget(TCPLogsWigtets *value)
//{
//    logWidget = value;
//}

bool TCPGetSpectrumsGistogramms::getWithRGU() const
{
    return withRGU;
}

void TCPGetSpectrumsGistogramms::setWithRGU(bool value, bool _toUpDown)
{
    withRGU = value;
    toUpDown = _toUpDown;
}

bool TCPGetSpectrumsGistogramms::getWithOffExp() const
{
    return withOffExp;
}

void TCPGetSpectrumsGistogramms::setWithOffExp(bool value)
{
    withOffExp = value;
}

TCPGetSpectrumsGistogramms::TCPGetSpectrumsGistogramms()
{
}

TCPGetSpectrumsGistogramms::TCPGetSpectrumsGistogramms(ServerConnect *_server, EnumCommands _dataType, SPRMainModel *_model, double _timeSpk_in_sec, QList<uint8_t> _threads, TCPTimeOutWigget *_toWidget, TCPLogsWigtets *_logWidgets):
    TCPCommandSet(_server, _toWidget, {}), model(_model), dataType(_dataType), timeOfSpectorScope_in_sec(_timeSpk_in_sec)
{
    workingThreads = _threads;
    withOffExp = false;
    withRGU = false;
}

QByteArray TCPGetSpectrumsGistogramms::getKSpectrumData(int thread){
    QVector<TCPCommand*>vspect = findCommands(getkspk);
    QByteArray res0, res;
    if(vspect.size() > 0){
        if(thread < 0 || thread >= vspect.size())
            thread = vspect.size()-1;

        res0 = vspect[thread]->getReplayData();
        //            res = res0.right(res0.size() - (5));
        res = res0.right(DEF_SPECTRUM_DATA_LENGTH_BYTE);
    }
    return res;
}

uint32_t TCPGetSpectrumsGistogramms::getKSpectrumTime(int thread){
    QVector<TCPCommand*>vspect = findCommands(getkspk);
    QByteArray res0; uint32_t res = 0;
    if(vspect.size() > 0){
        if(thread < 0 || thread >= vspect.size())
            thread = vspect.size()-1;

        res0 = vspect[thread]->getReplayData();
        res0 = res0.left(5);
        res0 = res0.right(4);
        memcpy(&res, res0.constData(), sizeof(res));
        //            res = res0.toInt();
    }
    return res;
}

QByteArray TCPGetSpectrumsGistogramms::getSpectrumData(int thread){
    QVector<TCPCommand*>vspect = findCommands(getspk);
    QByteArray res;
    if(vspect.size() > 0){
        if(thread < 0 || thread >= vspect.size())
            thread = vspect.size()-1;

        res = vspect[thread]->getReplayData().right(DEF_SPECTRUM_DATA_LENGTH_BYTE);
    }
    return res;
}

void TCPGetSpectrumsGistogramms::go(TCPCommand *_command)
{
    if(!_command){
        clear();

        if(dataType == getspk){
            char ren = '\0';
            if((!server->isState(spr_state_rentgen_on)) || (!server->isState(spr_state_separator_on))){
                addCommand(new TCPCommandSeparatorOnFull(server, model, widget));
            }
            if(withRGU){
                if(toUpDown){
                    addCommand(new TCPCommandRGUUpDown(server, getTimeOutWidget(), true));
                } else {
                    addCommand(new TCPCommandRGUUpDown(server, getTimeOutWidget(), false));
                }
            }
            if(!server->isState(spr_state_exposition_on)){
                if(server->isState(spr_state_rentgen_on_correct)){
                    addCommand(new TCPExpositionOnOff(server, false, true, model, getTimeOutWidget(), getLogWidget()));
                }
                addCommand(new TCPExpositionOnOff(server, true, true, model, getTimeOutWidget(), getLogWidget()));

            }
            int32_t spkT = timeOfSpectorScope_in_sec * 10;
            addCommand(new TCPCommand(setspk))->addSendData(&spkT, sizeof(spkT), setspk);
            addCommand(new TCPTimeOutCommand(timeoutcommand, (int)(timeOfSpectorScope_in_sec * 1000 + 2000), 100, widget,
                                MSG_TIME_OUT_SET_SPK, MSG_TIME_OUT_SET_SPK_MSG(timeOfSpectorScope_in_sec)));
//                                QString(tr("Накопление спектра...")),
//                                QString(tr("Накопление спектра за %1 сек...")).arg(timeOfSpectorScope_in_msec)));
        }

        uint8_t threads = MAX_SPR_MAIN_THREADS;
        if(model){
            threads = model->getSettingsMainModel()->getThreads()->getData();
        }
        if(workingThreads.size() > 0){
            threads = workingThreads.size();
        }

        if(dataType == getspk || dataType == getkspk || dataType == getgist){
            for(uint8_t i=0; i<threads; i++){
               addCommand(dataType)->addSendData(&i, sizeof(i), dataType);
//               findCommands(dataType).last()->setSendData(&i, sizeof(i));
            }
        } else {
            addCommand(dataType);
        }
        if(dataType == getspk){
            if(withOffExp){
                addCommand(new TCPExpositionOnOff(server, false, true, model, getTimeOutWidget(), getLogWidget()));
            }
        }

        addCommand(new TCPCommand(nocommand));

//        if(dataType == getspk){
//            char ren = '\0';
//            addCommand(new TCPCommand(expoff));
//            findCommands(expoff).last()->setSendData(&ren,sizeof(ren));
//            addCommand(new TCPCommand(offosw));
//        }
    } else {
        if(_command->getCommand() == getren){
            uint16_t mkv, mka; uint8_t err;
            bool res = _command->isRentgenReady(_command->getReplayData(), &mkv, &mka, &err);
            if(!res) {
                TCPExpositionOnOff *off = new TCPExpositionOnOff(server, false, true, model, getTimeOutWidget(), getLogWidget());
                addCommand(off);
                off->send(server);

                emit rentgenNotReady(_command);
                emit commandNotComplite(_command);
                return;
            }
        }
    }
    TCPCommandSet::go(_command);
}

//bool TCPGetSpectrumsGistogramms::isRentgenReady(uint *_mkv, uint *_mka){
//    QByteArray res = findCommands(getren).last()->getReplayData().right(4);
//    uint mka=0, mkv=0;
//    memcpy(&mkv, res.constData(), 2);
//    memcpy(&mka, res.constData()+2, 2);
//    if(_mkv) *_mkv = mkv;
//    if(_mka) *_mka = mka;
//    if(mkv >= 0x0600 && mka >= 0x0600 && mkv < 0x0700 && mka < 0x0700){
//        return true;
//    } else {
//        return false;
//    }

//}
