#include "tcpautosetrentgen.h"

bool TCPAutoSetRentgen::getUseRGU() const
{
    return useRGU;
}

void TCPAutoSetRentgen::setUseRGU(bool value)
{
    useRGU = value;
}

QList<int> TCPAutoSetRentgen::getThreads()
{
    return threads;
}

void TCPAutoSetRentgen::setThreads(QList<int> &value)
{
    threads = value;
}

void TCPAutoSetRentgen::setCodesDEU(QByteArray &value)
{
    codesDEU = value;
}

void TCPAutoSetRentgen::setCodesCP(QByteArray &value)
{
    codesCP = value;
}

void TCPAutoSetRentgen::settingCodes()
{
    findCommands(setudeu).last()->setSendData(codesDEU);
    findCommands(setptdeu).last()->setSendData(codesCP);
}

void TCPAutoSetRentgen::settingRentgenVA()
{
//    uint8_t rentgenNum = 1; /* DEF_SPR_MAIN_RENTGENS;*/
    QByteArray va;
    va.append("\0", 1);
    double dmkv = mainModel->getSettingsRentgenModel()->uTubes[0]->getData() * 21.8 - 23;
    double dmka = mainModel->getSettingsRentgenModel()->uTubes[0]->getData() * 23.4 - 101;
    uint16_t mkv = round(dmkv);
    uint16_t mka = round(dmka);
    va.append((char*)&mkv, sizeof(mkv));
    va.append((char*)&mka, sizeof(mka));

    findCommands(setren).last()->setSendData(va);
}

TCPAutoSetRentgen::TCPAutoSetRentgen(SPRMainModel *_model, TCPTimeOutWigget *_toWidget)
    :TCPCommandSet(nullptr, _toWidget, {}), mainModel(nullptr), separatorOn(nullptr)
{
    mainModel = _model;

    separatorOn = new TCPCommandSeparatorOnFull(mainModel->getServer(), mainModel, _toWidget);
    commandRGU = new TCPCommandRGUUpDown2(mainModel->getServer(), _toWidget, false);
}


void TCPAutoSetRentgen::go(TCPCommand *_command)
{
    if(!_command){
        clear();
        if(mainModel){
            if(!mainModel->getServer()->isState(spr_state_separator_on)){
//                addCommand(separatorOn);
                addCommand(new TCPCommand(initada));
                addCommand(new TCPCommand(onsep));
                addCommand(new TCPCommand(onren));

                addCommand(new TCPCommand(offosw));
                addCommand(new TCPCommand(onosw));

                uint timeHotTube = mainModel->getSettingsRentgenModel()->timeOnRA->getData();

                addCommand(new TCPTimeOutCommand(timeoutcommand, timeHotTube * 1000 + 1000,
                                                 10, widget, QString(tr("Включение рентгена...")),
                                                 QString(tr("Прогрев трубок (%1 секунд)...")).arg(timeHotTube)));
            }
            if(mainModel->getSettingsRentgenModel()->withRGU->getData()){
                addCommand(commandRGU);
            }
            char ch = 0;
            uint32_t spkTime = 0x64;
            if(mainModel->getServer()->isState(spr_state_rentgen_not_regime)){
                addCommand(expoff);
                addCommand(offosw);
            }

            addCommand(new TCPCommand(setren));
            settingRentgenVA();
            addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 5, widget, tr("Включение рентгена."), tr("Установка значений рентгена.")));

            addCommand(new TCPCommand(setudeu));
            addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 5, widget, tr("Включение рентгена."), tr("Установка кодов ДЭУ.")));
            addCommand(new TCPCommand(setptdeu));
            addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 5, widget, tr("Включение рентгена."), tr("Установка кодов СР.")));

            settingCodes();

            if(!mainModel->getServer()->isState(spr_state_exposition_on) || mainModel->getServer()->isState(spr_state_rentgen_not_regime)){

                addCommand(expon)->addCommand(offosw)->addCommand(onosw);
                findCommands(expon).last()->setSendData(&ch, 1);
                addCommand(new TCPTimeOutCommand(timeoutcommand, 6000, 100, getTimeOutWidget(), tr("Включение экспозиции"), tr("Включение экспозиции...")));
                addCommand(getren);
                findCommands(getren).last()->setSendData(&ch, 1);
            }

            addCommand(setspk);
            findCommands(setspk).last()->setSendData(&spkTime, sizeof(spkTime));
            addCommand(new TCPTimeOutCommand(timeoutcommand, 11000, 100, getTimeOutWidget(), tr("Накопление спектра"), tr("Накопление спектра (10 секунд)...")));
            foreach(int8_t thread, getThreads()){
                addCommand(getspk);
                findCommands(getspk).last()->setSendData(&thread, sizeof(thread));
            }
        }
    }
    TCPCommandSet::go(_command);
}
