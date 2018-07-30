#include "tcpcommandrentgeron.h"

void TCPCommandRentgerOn::setModel(SPRSettingsRentgenModel *value)
{
    if(value){
        model = value;

    }

}

TCPCommandRentgerOn::TCPCommandRentgerOn()
{

}

TCPCommandRentgerOn::TCPCommandRentgerOn(ServerConnect *_server, TCPTimeOutWigget *_widget, SPRSettingsRentgenModel *_model):
    TCPCommandSet(_server, _widget, {}), model(_model)
{
    command = setRentgenOn;
    rentgenNum = 1; /* DEF_SPR_MAIN_RENTGENS;*/
//    addCommand(new TCPCommand(onren)); // 0
//    addCommand(new TCPTimeOutCommand(timeoutcommand, DEF_SPR_RENTGEN_TIME_ON_RA*1000, 50, widget, tr("Включение рентгена."), tr("Разогрев трубок...")));
//    addCommand(new TCPCommand(nocommand)); // 2

//    addCommand(new TCPCommand(setren)); // 3
//    addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 5, widget, tr("Включение рентгена."), tr("Установка значений рентгена."))); // 4

//    addCommand(new TCPCommand(setudeu)); // 5
//    addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 5, widget, tr("Включение рентгена."), tr("Установка кодов ДЭУ."))); // 4
//    addCommand(new TCPCommand(setptdeu)); // 7
//    addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 5, widget, tr("Включение рентгена."), tr("Установка кодов СР."))); // 4
//    addCommand(new TCPTimeOutCommand(timeoutcommand, DEF_SPR_RENTGEN_TIME_ON_RA*1000*2, 50, widget, tr("Включение рентгена."), tr("Разогрев трубок...")));

//    QByteArray deuData;
//    QByteArray cpData;
//    for(uint8_t i=0; i<model->getThreads()->getData(); i++){
//        uint16_t d = model->deuCodes[i]->getData();
//        uint16_t cp = model->cpCodes[i]->getData();
//        deuData.append((char*)&d, sizeof(d));
//        cpData.append((char*)&cp, sizeof(cp));
//    }
//    findCommands(setudeu).last()->setSendData(deuData);
//    findCommands(setptdeu).last()->setSendData(cpData);


}

void TCPCommandRentgerOn::go(TCPCommand *_command)
{
    if(model){
        if(!_command){
//            while(commandSet.size() > 0){
//                commandSet.remove(0);
//            }
            clear();
            rentgenNum = 1; /* DEF_SPR_MAIN_RENTGENS;*/
            QByteArray va;
            va.append("\0", 1);
            double dmkv = model->uTubes[0]->getData() * 21.8 - 23;
            double dmka = model->uTubes[0]->getData() * 23.4 - 101;
            uint16_t mkv = round(dmkv);
            uint16_t mka = round(dmka);
//            uint16_t mkv = model->uTubes[0]->getData() * (-109) + 1176;
//            uint16_t mka = model->iTubes[0]->getData() * (-117) + 1186;
            va.append((char*)&mkv, sizeof(mkv));
            va.append((char*)&mka, sizeof(mka));

            addCommand(new TCPCommand(setren))->addSendData(va, lastcommand); // 3
//            findCommands(setren).last()->addSendData(va);

            addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 5, widget, tr("Включение рентгена."), tr("Установка значений рентгена."))); // 4

            QByteArray deuData;
            QByteArray cpData;
            for(uint8_t i=0; i<model->getThreads()->getData(); i++){
                uint16_t d = model->deuCodes[i]->getData();
                uint16_t cp = model->cpCodes[i]->getData();
                deuData.append((char*)&d, sizeof(d));
                cpData.append((char*)&cp, sizeof(cp));
            }
            addCommand(new TCPCommand(setudeu))->addSendData(deuData); // 5
            addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 5, widget, tr("Включение рентгена."), tr("Установка кодов ДЭУ."))); // 4
            addCommand(new TCPCommand(setptdeu))->addSendData(cpData); // 7
            addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 5, widget, tr("Включение рентгена."), tr("Установка кодов СР."))); // 4

//            findCommands(setudeu).last()->addSendData(deuData);
//            findCommands(setptdeu).last()->addSendData(cpData);
        }
        EnumCommands _name = nocommand;
        if(_command)
            _name = _command->getCommand();
        TCPCommandSet::go(_command);
    }
}
