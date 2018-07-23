#ifndef ITCPSEPARATORCOMMANDER_H
#define ITCPSEPARATORCOMMANDER_H

#include <QObject>
#include "tcp/TCPCommandSet.h"

enum SPRSeparatorCommand{
    separatorOn, // include rentgen
    separatorOff,
    rentgenOn, // include exposition and getren
    rentgenOff,
    separatedStart, // include separator, rentgen, control exposition
    separatedSet,
    separatedStop,

    getSpectrum, getKSpectrum, getSepar, getGistogramm,

    puwStart, puwtop, puwSet, // pitatel
    iwStart, iwStop, // rasklad

    termoGet, termoSet,

    testImsStart, testImsStop,
    rguUp, gruDown, rguGetState,
};


class ITCPSeparatorCommander : public TCPCommandSet
{
public:
    ITCPSeparatorCommander()
    {

    }
};

#endif // ITCPSEPARATORCOMMANDER_H
