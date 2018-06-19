#include "tcp/tcpstartstopseparate.h"
#include "tcp/tcpgetspectrumsgistogramms.h"
//#include "tcp/tcpgetrentgensparams.h"

void TCPStartStopSeparate::setModel(SPRMainModel *value)
{
    model = value;
}

TCPStartStopSeparate::TCPStartStopSeparate()
{
    
}

TCPStartStopSeparate::TCPStartStopSeparate(ServerConnect *_server, SPRMainModel _model, TCPTimeOutWigget *_widget):
    TCPCommandSet(_server, _widget, {}), running(false), model(nullptr)
{
    addCommand(new TCPCommand(getrgu2)); // 0
    addCommand(new TCPCommand(getblk)); // 1
    addCommand(new TCPCommand(expon)); // 2

    uint32_t timeOfSpk = 10;

    addCommand(new TCPCommand(offosw)); // 3
    addCommand(new TCPCommand(onosw)); // 4
    addCommand(new TCPTimeOutCommand(timeoutcommand, 10, 5, _widget, tr("Включение експозиции..."), tr("Включение експозиции..."))); // 5


//    addCommand(new TCPGetRentgensParams(_server)); // 6
    TCPGetSpectrumsGistogramms *spk = new TCPGetSpectrumsGistogramms(_server, setspk, model, _widget, getLogWidget());
    addCommand(spk); // 6
    commandSet.last()->setSendData((char*)&timeOfSpk, sizeof(timeOfSpk));

    // 7
    addCommand(new TCPTimeOutCommand(timeoutcommand, timeOfSpk / 10, 1, _widget, tr("Получение начальноых спектров"), tr("Получение начальноых спектров")));
    addCommand(new TCPCommand(getspk)); //8

    addCommand(new TCPCommand(setsepar)); //9
    addCommand(new TCPCommand(startsep)); // 10
    addCommand(new TCPTimeOutCommand(timeoutcommand, 2)); // 11

    addCommand(new TCPCommand(oniw)); // 12
    addCommand(new TCPCommand(setpuw)); // 13
    addCommand(new TCPCommand(startpuw)); // 14

    addCommand(new TCPTimeOutCommand(timeoutcommand, 1)); // 15 Cicling timeout;

    addCommand(new TCPCommand(getsepar)); // 16
    TCPGetSpectrumsGistogramms *kspk = new TCPGetSpectrumsGistogramms(_server, getkspk, model, _widget, getLogWidget());
    addCommand(kspk); // 17

    TCPGetSpectrumsGistogramms *gst = new TCPGetSpectrumsGistogramms(_server, getgist, model, _widget, getLogWidget());
    addCommand(gst); // 18

    addCommand(new TCPCommand(nocommand)); // 19

    addCommand(new TCPCommand(stoppuw)); // 20
    addCommand(new TCPTimeOutCommand(timeoutcommand, 10, 5, _widget, tr("Останов питателя..."), tr("Останов питателя..."))); // 21

    addCommand(new TCPCommand(offiw)); // 22
    addCommand(new TCPCommand(stopsep)); // 23
    addCommand(new TCPTimeOutCommand(timeoutcommand, 2)); // 24

    addCommand(new TCPCommand(expoff)); // 25
    addCommand(new TCPCommand(offosw)); // 26
    addCommand(new TCPCommand(onosw)); // 27
    addCommand(new TCPCommand(offosw)); // 28
    addCommand(new TCPCommand(onosw)); // 29


}

void TCPStartStopSeparate::go(TCPCommand *_command)
{
    if(!_command){
        threads = DEF_SPR_MAIN_THREADS, rentgens = DEF_SPR_MAIN_RENTGENS;
        tGetSeparate = DEF_SPR_CONTROL_T_MEASSURE_FOR_DATA;
        tGetSpectrum = DEF_SPR_CONTROL_T_MEASSURE_FOR_SPECTRUM;
        tGetGistogramm = DEF_SPR_CONTROL_T_MEASSURE_FOR_HISTOGRAMM;

        if(model){
            threads = model->getSettingsMainModel()->getThreads()->getData();
            rentgens = model->getSettingsMainModel()->getRentgens()->getData();

            tGetSeparate = model->getSettingsControlModel()->tMeassureForData->getData();
            tGetSpectrum = model->getSettingsControlModel()->tMeassureForSpectrum->getData();
            tGetGistogramm = model->getSettingsControlModel()->tMeassureForHistogramm->getData();
        }

        ((TCPGetSpectrumsGistogramms*)commandSet.at(7))->setThreadTimer(threads);
        ((TCPGetSpectrumsGistogramms*)commandSet.at(18))->setThreadTimer(threads);
        ((TCPGetSpectrumsGistogramms*)commandSet.at(19))->setThreadTimer(threads);

//        ((TCPGetRentgensParams*)commandSet.at(19))->setRentgens(rentgens);

        running = true;
        timeOutsCount = 0;

        commandSet[0]->send(server);
        return;

    } else {
        int numCommand = _command->getNum();

//        if(numCommand == 6){ // getren command
//            TCPGetRentgensParams *grg = (TCPGetRentgensParams *)(findCommands(getren).first());
//            uint V = grg->getVARentgen(true);
//            uint A = grg->getVARentgen(false);
//            commandSet[numCommand+1]->send(server);
//            return;
//        }
        if(numCommand == 15){ // TimeOut Periodical Commands
            timeOutsCount++; if(timeOutsCount > 100000) timeOutsCount = 1;
            uint timeValue = timeOutsCount % tGetSeparate;
            if(timeValue == 0){
                commandSet[17]->send(server);
            }
            timeValue = timeOutsCount % tGetSpectrum;
            if(timeValue == 0){
                commandSet[17]->send(server);
            }
            timeValue == timeOutsCount % tGetGistogramm;
            if(timeValue == 0){
                commandSet[18]->send(server);
            }

            commandSet[19]->send(server);
            return;
        }
        if(numCommand == 16 || numCommand == 17|| numCommand == 18){
            commandSet[19]->send(server);
            return;
        }
        if(numCommand == 19){
            if(running){
                commandSet[15]->send(server);
            } else {
                commandSet[numCommand+1]->send(server);
            }
            return;
        }
        if(numCommand < commandSet.size() - 1){
            commandSet[numCommand+1]->send(server);
        } else {
            emit commandComplite(this);
        }
    }

}
