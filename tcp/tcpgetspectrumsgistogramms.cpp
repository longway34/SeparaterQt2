#include "tcpgetspectrumsgistogramms.h"
#include "tcp/tcpexpositionoff.h"

void TCPGetSpectrumsGistogramms::setThreadTimer(uint _threadsNum, double _time_in_sec, QList<uint8_t> _wtList)
{
    /*threadNum = _threadsNum;*/ timeOfSpectorScope = _time_in_sec;

    workingThreads.clear();
    for(int8_t i=0; i<MAX_SPR_MAIN_THREADS; i++){
        if(_wtList.empty() || _wtList.contains(i)){
             workingThreads.push_back(i);
        }
    }

    for(int i=0; i< commandSet.size(); i++){
        delete commandSet[i];
    }
    commandSet.clear();

       int tTimer = 1;
       uint32_t tArg = timeOfSpectorScope * 10;
       if(dataType == getspk){
           TCPCommand *setRen = new TCPCommand(setspk);
//           QByteArray data;
//           data.append((char*)&tArg, sizeof(tArg));
           setRen->setSendData(&tArg, sizeof(tArg));
           addCommand(setRen);
           addCommand(new TCPTimeOutCommand(timeoutcommand, timeOfSpectorScope*1000+2000, timeOfSpectorScope, this->widget, tr("Накопление спектра"), tr("Накопление спектра")));
       }
       for(uint8_t i=0; i<workingThreads.size(); i++){
           TCPCommand *comm = new TCPCommand(dataType);
//           uint8_t arg = i;
//           QByteArray data;
//           data.append((char*)&arg, sizeof(arg));
           uint8_t num = workingThreads[i];
           comm->setSendData(&num, sizeof(num));
           addCommand(comm);
       }
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

TCPGetSpectrumsGistogramms::TCPGetSpectrumsGistogramms()
{
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

TCPGetSpectrumsGistogramms::TCPGetSpectrumsGistogramms(ServerConnect *_server, EnumCommands _dataType, SPRMainModel *_model, TCPTimeOutWigget *_widget, TCPLogsWigtets *_logWidget):
    TCPCommandSet(_server, _widget, {}), model(_model), dataType(_dataType)
{
    command = setGetSpectrumsGistorfamms;
    setName(tr("setGetSpectrumsGistorfamms"));
    setLogWidget(_logWidget);
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
            if(!server->isState(spr_state_exposition_on)){
                addCommand(new TCPCommand(expon));
                findCommands(expon).last()->setSendData(&ren, sizeof(ren));

                addCommand(new TCPCommand(offosw));
                addCommand(new TCPCommand(onosw));
                uint expTo = DEF_SPR_RENTGEN_TIME_HOT_RA;
                if(model){
                    expTo = model->getSettingsRentgenModel()->timeHotRA->getData();
                }

                addCommand(new TCPTimeOutCommand(timeoutcommand, expTo*1000+2000, 10, widget,
                                    QString(tr("Включение экспозиции...")),
                                    QString(tr("Включение экспозиции (%1 секунд)")).arg(expTo)));
                addCommand(new TCPCommand(getren));
                findCommands(getren).last()->setSendData(&ren, sizeof(ren));
            }
            addCommand(new TCPCommand(setspk));
            int32_t spkT = timeOfSpectorScope * 10;
            findCommands(setspk).last()->setSendData(&spkT, sizeof(spkT));
            addCommand(new TCPTimeOutCommand(timeoutcommand, timeOfSpectorScope * 1000+2000, 100, widget,
                                QString(tr("Накопление спектра...")),
                                QString(tr("Накопление спектра за %1 секунд...")).arg(timeOfSpectorScope)));
        }

        uint8_t threads = MAX_SPR_MAIN_THREADS;
        if(model){
            threads = model->getSettingsMainModel()->getThreads()->getData();
        }

        if(dataType == getspk || dataType == getkspk || dataType == getgist){
            for(uint8_t i=0; i<threads; i++){
               addCommand(dataType);
               findCommands(dataType).last()->setSendData(&i, sizeof(i));
            }
        } else {
            addCommand(dataType);
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
            uint mkv, mka;
            bool res = isRentgenReady(&mkv, &mka);
            TCPLogsWigtets *lw = getLogWidget();
            if(res){
                if(lw){
                    lw->onLogsCommand(nullptr, QString(tr("Рентген вышел на рабочий режим... (%1 mkV, %2 mkA)")).arg(QString::number(mkv, 16)).arg(QString::number(mka, 16)));
                }
                emit rentgenReady(_command);
            } else {
                if(lw){
                    lw->onErrorLogsCommand(nullptr, QString(tr("Рентген не вышел на рабочий режим... (%1 mkV, %2 mkA)")).arg(QString::number(mkv, 16)).arg(QString::number(mka, 16)));
                }
                emit rentgenNotReady(_command);
//                _command = findCommands(nocommand).last();
                char ch='\0';
                TCPExpositionOff *exp = new TCPExpositionOff(getLogWidget());
//                exp->setSendData(&ch, sizeof(ch));
//                addCommand(expoff);
                exp->send(model->getServer());
                emit commandNotComplite(_command);
                return;
            }
        }
    }
    TCPCommandSet::go(_command);
}

bool TCPGetSpectrumsGistogramms::isRentgenReady(uint *_mkv, uint *_mka){
    QByteArray res = findCommands(getren).last()->getReplayData().right(4);
    uint mka=0, mkv=0;
    memcpy(&mkv, res.constData(), 2);
    memcpy(&mka, res.constData()+2, 2);
    if(_mkv) *_mkv = mkv;
    if(_mka) *_mka = mka;
    if(mkv >= 0x0600 && mka >= 0x0600 && mkv < 0x0700 && mka < 0x0700){
        return true;
    } else {
        return false;
    }

}
