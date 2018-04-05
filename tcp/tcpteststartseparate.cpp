#include "tcpteststartseparate.h"
#include "tcp/tcpcommandrentgeron.h"
#include "tcp/tcpcommandseparatoron.h"
#include "tcp/tcpgetspectrumsgistogramms.h"

TCPTestStartSeparate::TCPTestStartSeparate(ServerConnect *_server, TCPTimeOutWigget *_widget):
    TCPCommandSet(server, _widget, {}), separateModel(nullptr)
{
    char ch0 = '\0';
    addCommand(new TCPCommandSeparatorOn(_server, _widget));
    addCommand(new TCPCommandRentgerOn(_server, _widget));

    addCommand(new TCPCommand(expon))->setSendData(&ch0, sizeof(ch0));
    addCommand(new TCPCommand(offosw));
    addCommand(new TCPCommand(onosw));

    addCommand(new TCPTimeOutCommand(timeoutcommand, 10, 10, _widget, tr("Включение эхспозиции"), tr("Включение эхспозиции")));
    addCommand(new TCPCommand(getren))->setSendData(&ch0, sizeof(ch0));

    TCPGetSpectrumsGistogramms *spect = new TCPGetSpectrumsGistogramms(_server, getspk);
    addCommand(spect);

    addCommand(new TCPCommand(setsepar));
    addCommand(new TCPCommand(startsep));

    addCommand(new TCPCommand(oniw));

}

void TCPTestStartSeparate::go(TCPCommand *_command)
{
    if(!_command){
        if(separateModel){
            findCommands(setsepar).first()->setSendData(separateModel->toByteArray());
        }
    }
    TCPCommandSet::go(_command);
}

SPRSeparateModel *TCPTestStartSeparate::getSeparateModel() const
{
    return separateModel;
}

void TCPTestStartSeparate::setSeparateModel(SPRSeparateModel *value)
{
    separateModel = value;
    TCPCommandRentgerOn *reng = ((TCPCommandRentgerOn*)commandSet[1]);
    reng->setModel(separateModel->getMainModel()->getSettingsRentgenModel());

}

SPRMainModel *TCPTestStartSeparate::getMainModel() const
{
    if(separateModel){
        return separateModel->getMainModel();
    }
    return nullptr;
}


