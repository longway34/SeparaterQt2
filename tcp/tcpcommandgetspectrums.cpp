#include "tcpcommandgetspectrums.h"

int TCPCommandGetSpectrums::getTimeOfSpectrum() const
{
    return timeOfSpectrum;
}

void TCPCommandGetSpectrums::setTimeOfSpectrum(int value)
{
    timeOfSpectrum = value > 0 ? value: 5;
}

uint8_t TCPCommandGetSpectrums::getChannels() const
{
    return channels;
}

void TCPCommandGetSpectrums::setModel(SPRMainModel *value)
{
    model = value;
}

TCPCommandGetSpectrums::TCPCommandGetSpectrums()
{

}

TCPCommandGetSpectrums::TCPCommandGetSpectrums(ServerConnect *_server, TCPTimeOutWigget *_widget, SPRMainModel *_model, int _time)
    :TCPCommandSet(_server, _widget, {}), model(_model)
{
    setTimeOfSpectrum(_time);

    addCommand(new TCPCommand(getblk)); //0
    addCommand(new TCPCommand(expon)); // 1
    QByteArray chData = QByteArray::fromRawData("\x00", 1);
    findCommands(expon).last()->setSendData(chData);

    addCommand(new TCPCommand(offosw)); // 2
    addCommand(new TCPCommand(onosw)); // 3
    addCommand(new TCPTimeOutCommand(timeoutcommand, DEF_SPR_RENTGEN_TIME_HOT_RA*2000, 10, widget, tr("Включение рентгена"), tr("Установка экспозиции...")));
    addCommand(new TCPCommand(getren)); //5
    findCommands(getren).last()->setSendData(chData);
    addCommand(new TCPCommand(setspk)); //6
    addCommand(new TCPTimeOutCommand(timeoutcommand, timeOfSpectrum, 10, widget, tr("Накопление спектра"), tr("Накопление спектра")));
    addCommand(new TCPCommand(nocommand)); // 8

    addCommand(new TCPCommand(getspk)); // 9

    addCommand(new TCPCommand(expoff)); // 10
    findCommands(expoff).last()->setSendData(chData);
    addCommand(new TCPCommand(offosw)); // 11
    addCommand(new TCPCommand(onosw)); // 12
    addCommand(new TCPCommand(offosw)); // 13
    addCommand(new TCPCommand(onosw)); // 14
}

TCPCommand *cDeb;

void TCPCommandGetSpectrums::go(TCPCommand *_command)
{
    if(!_command){
        spectrumsData.clear();
        channelCount = 0;
        channels = DEF_SPR_MAIN_THREADS;
        uint hotTime = DEF_SPR_RENTGEN_TIME_HOT_RA * 1000;

        if(model){
            channels = model->getSettingsMainModel()->getThreads()->getData();
            hotTime = model->getSettingsRentgenModel()->timeHotRA->getData() * 1000;
        }

        findCommands(timeoutcommand).first()->setTimeOut(hotTime);
        uint32_t spkTime = timeOfSpectrum * 10;
        QByteArray baSpk = QByteArray::fromRawData((char*)&spkTime, sizeof(spkTime));
        cDeb = findCommands(setspk).first();
        findCommands(setspk).first()->setSendData(baSpk);
        QByteArray rrr = findCommands(setspk).first()->getSendData();
        TCPTimeOutCommand *to2 = (TCPTimeOutCommand*)(findCommands(timeoutcommand).last());
        to2->setTimeOut(timeOfSpectrum * 1000+1000);
        to2->setWidgetTitleMessage(tr("Накопление спектра"), QString(tr("Идет накопление спектра за %1 секунд\nПодождите...")).arg(QString::number(timeOfSpectrum)));

        findCommands(getspk).first()->setSendData(QByteArray::fromRawData("\x00", 1));

        commandSet[0]->send(server);
        return;
    } else if(_command->getNum() >= commandSet.size() -1){
        emit commandComplite(this);
        return;
    } else {
        int num = _command->getNum();
        if(num == 9){
            spectrumsData.push_back(_command->getResult());
            if(channelCount < channels - 1){
                findCommands(getspk).first()->setSendData(QByteArray::fromRawData((char*)&channelCount, sizeof(channelCount)));
                channelCount++;
                findCommands(getspk).first()->send(server);
                return;
            } else {
                commandSet[num + 1]->send(server);
                return;
            }
            return;
        } else {
            if(commandSet[num + 1]->getCommand() == setspk){
                cDeb = commandSet[num + 1];
                TCPCommand *_comm = commandSet[num + 1];
                uint32_t spkTime = timeOfSpectrum * 10;
                QByteArray baSpk = QByteArray::fromRawData((char*)&spkTime, sizeof(spkTime));
                _comm->setSendData(baSpk);
                int x = 0;
            }
            commandSet[num + 1]->send(server);
            return;
        }
    }
}




