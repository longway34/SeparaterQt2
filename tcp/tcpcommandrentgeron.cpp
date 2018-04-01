#include "tcpcommandrentgeron.h"

void TCPCommandRentgerOn::setModel(SPRSettingsRentgenModel *value)
{
    model = value;
}

TCPCommandRentgerOn::TCPCommandRentgerOn()
{

}

TCPCommandRentgerOn::TCPCommandRentgerOn(ServerConnect *_server, TCPTimeOutWigget *_widget, SPRSettingsRentgenModel *_model):
    TCPCommandSet(_server, _widget, {}), model(_model)
{
    rentgenNum = DEF_SPR_MAIN_RENTGENS;
    addCommand(new TCPCommand(onren)); // 0
    addCommand(new TCPTimeOutCommand(timeoutcommand, DEF_SPR_RENTGEN_TIME_ON_RA*1000, 5, widget, tr("Включение рентгена."), tr("Ожидание включения рентгена.")));
    addCommand(new TCPCommand(nocommand)); // 2

    addCommand(new TCPCommand(setren)); // 3
    addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 5, widget, tr("Включение рентгена."), tr("Установка значений тока и напряжения."))); // 4

    addCommand(new TCPCommand(setudeu)); // 5
    addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 5, widget, tr("Включение рентгена."), tr("Установка кодов ДЭУ."))); // 4
    addCommand(new TCPCommand(setptdeu)); // 7
    addCommand(new TCPTimeOutCommand(timeoutcommand, 2000, 5, widget, tr("Включение рентгена."), tr("Установка кодов СР."))); // 4

//    addCommand(new TCPCommand(setudeu));
//    addCommand(new TCPTimeOutCommand(timeoutcommand, 500, 1));
//    addCommand(new TCPCommand(setptdeu));
//    addCommand(new TCPTimeOutCommand(timeoutcommand, 500, 1));


}

void TCPCommandRentgerOn::go(TCPCommand *_command)
{
    if(!_command){
        QByteArray deuData;
        QByteArray cpData;
        uint16_t d = DEF_SPR_RENTGEN_DEU_CODE;
        uint16_t cp = DEF_SPR_RENTGEN_CP_CODE;
        uint8_t threads = DEF_SPR_MAIN_THREADS;
        rentgenNum = DEF_SPR_MAIN_RENTGENS;
        if(model){
            threads = model->getThreads()->getData();
            rentgenNum = model->getRentgens()->getData();
        }
        for(uint8_t i=0; i<threads; i++){
            if(model){
                d = model->deuCodes[i]->getData();
                cp = model->cpCodes[i]->getData();
            }
            deuData.append(QByteArray::fromRawData((char*)&d, sizeof(d)));
            cpData.append(QByteArray::fromRawData((char*)&cp, sizeof(cp)));
        }
        findCommands(setudeu).last()->setSendData(deuData);
        findCommands(setptdeu).last()->setSendData(cpData);

        rentgenCount = 0;
        commandSet[0]->send(server);
        return;
    } else {
        int num = _command->getNum();
        if(num == 2){ // first timeout and nocommand
            uint16_t kv;
            uint16_t mka;
            QByteArray data;
            if(model){
                kv = model->uTubes[rentgenCount]->getData();
                mka = model->iTubes[rentgenCount]->getData();
            } else {
                kv = DEF_SPR_RENTGEN_U_TUBE;
                mka = DEF_SPR_RENTGEN_I_TUBE;
            }
            kv = kv * (21.8) - 23;
            mka = mka * (23.4) - 101;
            data.append(QByteArray::fromRawData((char*)&rentgenCount, 1));
            data.append(QByteArray::fromRawData((char*)&kv, sizeof(kv)));
            data.append(QByteArray::fromRawData((char*)&mka, sizeof(mka)));

            findCommands(setren).last()->setSendData(data);

            commandSet[num + 1]->send(server);
            return;
        }
        if(num == 4){ // setren command and timeout
            rentgenCount++;
            if(rentgenCount >= rentgenNum){
                commandSet[5]->send(server);
                return;
            } else {
                commandSet[2]->send(server);
                return;
            }
        }
        if(num < commandSet.size()-1){
            commandSet[num + 1]->send(server);
            return;
        } else {
            emit commandComplite(this);
            return;
        }
    }
}
