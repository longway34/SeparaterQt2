#include "tcpteststopseparate.h"

TCPTestStopSeparate::TCPTestStopSeparate():
    TCPCommandSet(nullptr, nullptr, {})
{
}

TCPTestStopSeparate::TCPTestStopSeparate(TCPTimeOutWigget *_widget, SPRMainModel *_model):
    TCPCommandSet(nullptr, _widget, {}), model(nullptr)
{
    setModelData(_model);
}

void TCPTestStopSeparate::setModelData(SPRMainModel *_model)
{
    model = _model;
}


void TCPTestStopSeparate::go(TCPCommand *_command)
{
    if(!_command){
        clear();

        command = setTestStopSeparate;
        addCommand(new TCPCommand(stoppuw));
        addCommand(new TCPTimeOutCommand(timeoutcommand, 10*1000, 10, widget,
                                         tr("Останов сепаратора"),tr("Останов сепаратора")));

        char ch0 = '\0';
        addCommand(new TCPCommand(offiw));
        addCommand(new TCPCommand(stopsep));
        TCPCommand *exp = new TCPCommand(expoff); exp->setSendData(&ch0, sizeof(ch0));
        addCommand(exp);
        addCommand(new TCPCommand(offren));
        int tOffRentgen = DEF_SPR_RENTGEN_TIME_OFF_RA;
        if(model){
            tOffRentgen = model->getSettingsRentgenModel()->timeOffRA->getData();
        }
        addCommand(new TCPTimeOutCommand(timeoutcommand, tOffRentgen * 1000, 10, widget,
                                        tr("Останов рентгена"),
                                        QString(tr("Останов рентгена (%1 секунд)...")).arg(tOffRentgen)));
        addCommand(new TCPCommand(offosw));
        addCommand(new TCPCommand(onosw));
        addCommand(new TCPCommand(offosw));
        addCommand(new TCPCommand(onosw));
        addCommand(new TCPCommand(offosw));
        addCommand(new TCPCommand(offsep));
    }
    TCPCommandSet::go(_command);
}
