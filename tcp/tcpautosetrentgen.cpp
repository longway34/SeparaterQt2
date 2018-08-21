#include "tcpautosetrentgen.h"

bool TCPAutoSetRentgen::getUseRGU() const
{
    return useRGU;
}

void TCPAutoSetRentgen::setUseRGU(bool value)
{
    useRGU = value;
}

SPRThreadList TCPAutoSetRentgen::getThreads()
{
    return threads;
}

void TCPAutoSetRentgen::setThreads(SPRThreadList &value)
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
    findCommands(setudeu).last()->addSendData(codesDEU);
    findCommands(setptdeu).last()->addSendData(codesCP);
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

    findCommands(setren).last()->addSendData(va);
}

TCPAutoSetRentgen::TCPAutoSetRentgen(SPRMainModel *_model, TCPTimeOutWigget *_toWidget)
    :TCPCommandSet(nullptr, _toWidget, {}), mainModel(nullptr), separatorOn(nullptr)
{
    mainModel = _model;

    separatorOn = new TCPCommandSeparatorOnFull(mainModel->getServer(), mainModel, _toWidget);
    commandRGU = new TCPCommandRGUUpDown2(mainModel->getServer(), _toWidget, false);
}

#define _MULAGE_DEBUG

void TCPAutoSetRentgen::go(TCPCommand *_command)
{
#ifndef _MULAGE_DEBUG
            uint time_out_hot_tube = mainModel->getSettingsRentgenModel()->timeOnRA->getData();
            uint _time_out_expon = mainModel->getSettingsRentgenModel()->timeHotRA->getData() * 1000 + 1000;
            uint _time_out_on_deu_cp = 2000;
            uint32_t spkTime = 0x64;
            uint _time_out_spk = 11000;

#else
            uint time_out_hot_tube = 1;
            uint _time_out_expon = 2000;
            uint _time_out_on_deu_cp = 1000;
            uint32_t spkTime = 0x64;
            uint _time_out_spk = 2000;
#endif
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

                addCommand(new TCPTimeOutCommand(timeoutcommand, time_out_hot_tube * 1000 + 1000,
                                                 10, widget,
                                                 MSG_TIME_OUT_EXP_ON,
                                                 MSG_TIME_OUT_REN_ON_MSG(time_out_hot_tube / 1000)));
                                                 /*QString(tr("Прогрев трубок (%1 секунд)...")).arg(time_out_hot_tube / 1000)));*/
            }
            if(mainModel->getSettingsRentgenModel()->withRGU->getData()){
                addCommand(commandRGU);
            }
            char ch = 0;
            if(mainModel->getServer()->isState(spr_state_rentgen_not_regime)){
                addCommand(expoff);
                findCommands(expoff).last()->addSendData(&ch, 1);
                addCommand(offosw);
            }

            addCommand(new TCPCommand(setren));
            settingRentgenVA();



            addCommand(new TCPTimeOutCommand(timeoutcommand, _time_out_on_deu_cp, 100, getTimeOutWidget(),
                            MSG_TIME_OUT_REN_ON, MSG_TIME_OUT_EXP_ON_MSG(_time_out_on_deu_cp / 1000)));
//                                             tr("Включение рентгена."),
//                                             QString(tr("Установка значений рентгена. (%1 сек.)")).arg(_time_out_on_deu_cp / 1000)));

            addCommand(new TCPCommand(setudeu));
            addCommand(new TCPTimeOutCommand(timeoutcommand, _time_out_on_deu_cp, 100, getTimeOutWidget(),
                            MSG_TIME_OUT_SET_DEU, MSG_TIME_OUT_SET_DEU_MSG(_time_out_on_deu_cp / 1000)));
//                          tr("Включение рентгена."), QString(tr("Установка кодов ДЭУ. (%1 сек.)")).arg(_time_out_on_deu_cp / 1000)));
            addCommand(new TCPCommand(setptdeu));
            addCommand(new TCPTimeOutCommand(timeoutcommand, _time_out_on_deu_cp, 100, getTimeOutWidget(),
                            MSG_TIME_OUT_SET_CP, MSG_TIME_OUT_SET_CP_MSG(_time_out_on_deu_cp / 1000)));
//                            tr("Включение рентгена."), QString(tr("Установка кодов СР. (%1 сек.)")).arg(_time_out_on_deu_cp / 1000)));

            settingCodes();

            if(!mainModel->getServer()->isState(spr_state_exposition_on) && !mainModel->getServer()->isState(spr_state_rentgen_on_correct)){

                addCommand(expon)->addCommand(offosw)->addCommand(onosw);
                findCommands(expon).last()->addSendData(&ch, 1);
                addCommand(new TCPTimeOutCommand(timeoutcommand, _time_out_expon, 100, getTimeOutWidget(),
                            MSG_TIME_OUT_EXP_ON, MSG_TIME_OUT_EXP_ON_MSG(_time_out_on_deu_cp / 1000)));
//                            tr("Включение экспозиции"), QString(tr("Включение экспозиции... (%1 сек.)")).arg(_time_out_on_deu_cp / 1000)));
                addCommand(getren);
                findCommands(getren).last()->addSendData(&ch, 1);
            }
            addCommand(setspk);
            findCommands(setspk).last()->addSendData(&spkTime, sizeof(spkTime));
            addCommand(new TCPTimeOutCommand(timeoutcommand, _time_out_spk, 100, getTimeOutWidget(),
                            MSG_TIME_OUT_SET_SPK, MSG_TIME_OUT_SET_SPK_MSG(_time_out_spk / 1000)));
//                            tr("Накопление спектра"), QString(tr("Накопление спектра  (%1 сек.)")).arg(_time_out_spk / 1000)));
            foreach(int8_t thread, getThreads()){
                addCommand(getspk);
                findCommands(getspk).last()->addSendData(&thread, sizeof(thread));
            }
        }
    }
    TCPCommandSet::go(_command);
}
