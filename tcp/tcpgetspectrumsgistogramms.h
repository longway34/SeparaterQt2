#ifndef TCPGETSPECTRUMSGISTORGAMMS_H
#define TCPGETSPECTRUMSGISTORGAMMS_H

#include <QObject>
#include "tcp/TCPCommandSet.h"
#include "tcp/tcpcommandrentgenonfull.h"
#include "models/sprsettingsrentgenmodel.h"
#include "tcp/tcplogswigtets.h"
#include "tcp/tcpexpositiononoff.h"

#include "_types.h"

class TCPGetSpectrumsGistogramms : public TCPCommandSet
{
    Q_OBJECT
    
//    uint threadNum;
    QList<uint8_t> workingThreads;
    EnumCommands dataType;
    double timeOfSpectorScope_in_sec;
    bool withRGU;
    bool withOffExp;
    bool toUpDown;

    SPRMainModel *model;
//    TCPLogsWigtets *logWidget;

public:
    TCPGetSpectrumsGistogramms();
//    TCPGetSpectrumsGistogramms(ServerConnect *_server, EnumCommands _dataType, SPRMainModel *_model, TCPTimeOutWigget *_widget = nullptr, TCPLogsWigtets *_logWidget = nullptr);
    TCPGetSpectrumsGistogramms(ServerConnect *_server, EnumCommands _dataType, SPRMainModel *_model, double _timeSpk_in_sec = -1, QList<uint8_t> _threads = {}, TCPTimeOutWigget *_toWidget=nullptr, TCPLogsWigtets *_logWidgets=nullptr);

    void setModelData(SPRMainModel *_model){
        model = _model;
    }

    QByteArray getKSpectrumData(int thread = -1);
    uint32_t getKSpectrumTime(int thread);
    QByteArray getSpectrumData(int thread);

    QByteArray getGistogrammData(int thread){
        QVector<TCPCommand*> vcomm = findCommands(getgist);
        QByteArray res;
        if(vcomm.size() > 0){
            if(thread < 0 || thread >= vcomm.size())
                thread = vcomm.size()-1;

            res = vcomm[thread]->getReplayData();
        }
        return res;
    }

    void setThreadTimer(double _time_spk_in_sec = 1, QList<uint8_t> _wtList = {});

    virtual EnumCommands getDataType() const;

//    bool isRentgenReady(uint *_mkv = nullptr, uint *_mka = nullptr);
//    TCPLogsWigtets *getLogWidget() const;
//    virtual void setLogWidget(TCPLogsWigtets *value);

    bool getWithRGU() const;
    void setWithRGU(bool value, bool _toUpDown=false);

    bool getWithOffExp() const;
    void setWithOffExp(bool value);

protected slots:
    virtual void go(TCPCommand *_command = NULL);

signals:
    void rentgenReady(TCPCommand*);
    void rentgenNotReady(TCPCommand*);
};

#endif // TCPGETSPECTRUMSGISTORGAMMS_H
