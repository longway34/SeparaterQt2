#-------------------------------------------------
#
# Project created by QtCreator 2017-12-19T12:57:18
#
#-------------------------------------------------

QT       += core gui xml network
CONFIG   += qwt

win32:CONFIG += console
win32:TARGET  = ../XmlDOMRead

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Separator7
TEMPLATE = app

#QMAKE_CXXFLAGS_DEBUG += -g3 -pg
#QMAKE_LFLAGS_DEBUG += -pg -lgmon

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    _types.cpp \
    colorbutton.cpp \
    ipvalidator.cpp \
    isprcontroller.cpp \
    isprwidget.cpp \
    main.cpp \
    sprformulaelement.cpp \
    sprformulaitemwidget.cpp \
    sprporogswidget.cpp \
    sprsettingscontrolwidget.cpp \
    sprsettingsformulawidget.cpp \
    sprsettingsimswidget.cpp \
    sprsettingsmainwidget.cpp \
    sprsettingsporogswidget.cpp \
    sprsettingsrentgenautosetwidget.cpp \
    sprsettingsrentgencontrolwidget.cpp \
    sprsettingsrentgenmainwidget.cpp \
    sprsettingsrentgenwidget.cpp \
    sprspectrumbasedlisttable.cpp \
    sprspectrumlisttablewidget.cpp \
    sprspectrumtableadv.cpp \
    models/imodelvariable.cpp \
    models/isprmodeldata.cpp \
    models/sprformulaitemmodel.cpp \
    models/sprgrspectrumitemmodel.cpp \
    models/sprporogsmodel.cpp \
    models/sprsettingscontrolmodel.cpp \
    models/sprsettingsformulamodel.cpp \
    models/sprsettingsimsmodel.cpp \
    models/sprsettingsmainmodel.cpp \
    models/sprsettingsporogsmodel.cpp \
    models/sprsettingsrentgenmodel.cpp \
    models/sprsettingsrmtautosetmodel.cpp \
    models/sprsettingsspectrumrangesmodel.cpp \
    verticallabel.cpp \
    maintabwidget.cpp \
    sprsettingswidget.cpp \
    models/sprmainmodel.cpp \
    models/sprspectrumzonestablemodel.cpp \
    models/sprspectrumzonesmodel.cpp \
    models/sprspectrumitemmodel.cpp \
    models/sprspectrumlistitemsmodel.cpp \
    testtablewidget.cpp \
    sprspectrumgraphicswidget.cpp \
    sprtestimswidget.cpp \
    tcp/ITCPCommand.cpp \
    tcp/ServerConnect.cpp \
    tcp/TCPTimeOutCommand.cpp \
    tcp/TCPCommand.cpp \
    tcp/tcptimeoutwigget.cpp \
    tcp/TCPCommandSet.cpp \
    tcp/tcplogswigtets.cpp \
    tcp/tcpcommandrguup.cpp \
    tcp/tcpcommandrguupdown.cpp \
    tcp/tcpcommandrentgeron.cpp \
    tcp/tcpcommandgetspectrums.cpp \
    tcp/tcpstartsoptestimscommand.cpp \
    tcp/tcpcommandstartpitatel.cpp \
    tcp/tcpstartrudospuskcommand.cpp \
    tcp/tcpstoprudostuskcommand.cpp \
    tcp/tcpcommandstoppitatel.cpp \
    models/sprseparatemodel.cpp \
    sprseparatewidget.cpp \
    tcp/tcpstartstopseparate.cpp \
    tcp/tcpgetspectrumsgistogramms.cpp \
    tcp/tcpgetrentgensparams.cpp \
    sprspectrumsmalltable.cpp \
    tcp/tcpteststartseparate.cpp \
    tcp/tcpteststopseparate.cpp \
    tcp/tcpseparatego.cpp \
    sprspectrumbaselisttable2.cpp \
    models/sprelementsmodel.cpp \
    variables/sprdatetimevariable.cpp \
    variables/sprenumvariable.cpp \
    variables/sprqcolorvariable.cpp \
    variables/sprqstringvariable.cpp \
    variables/sprvariable.cpp \
    variables/sprkprvariable.cpp \
    variables/sprbooleanvariable.cpp \
    firstcollumn2.cpp \
    sprelementpropertydialog.cpp \
    sprzonesranges.cpp \
    sprzonesrangestable.cpp \
    sprzonesrangestablewidget.cpp \
    sprspectrumlisttable.cpp \
    sprspectrumchoisetimedialog.cpp \
    sprdeucodestable.cpp \
    tcp/tcpcommandrentgenonfull.cpp \
    tcp/tcpcommandseparatoronoff.cpp \
    models/imainmodel.cpp \
    sprrentgenvatable.cpp

HEADERS += \
    _types.h \
    colorbutton.h \
    ipvalidator.h \
    isprcontroller.h \
    isprwidget.h \
    sprformulaelement.h \
    sprformulaitemwidget.h \
    sprporogswidget.h \
    sprsettingscontrolwidget.h \
    sprsettingsformulawidget.h \
    sprsettingsimswidget.h \
    sprsettingsmainwidget.h \
    sprsettingsporogswidget.h \
    sprsettingsrentgenautosetwidget.h \
    sprsettingsrentgencontrolwidget.h \
    sprsettingsrentgenmainwidget.h \
    sprsettingsrentgenwidget.h \
    sprspectrumbasedlisttable.h \
    sprspectrumtableadv.h \
    models/imodelvariable.h \
    models/isprmodeldata.h \
    models/sprformulaitemmodel.h \
    models/sprgrspectrumitemmodel.h \
    models/sprporogsmodel.h \
    models/sprsettingscontrolmodel.h \
    models/sprsettingsformulamodel.h \
    models/sprsettingsimsmodel.h \
    models/sprsettingsmainmodel.h \
    models/sprsettingsporogsmodel.h \
    models/sprsettingsrentgenmodel.h \
    models/sprsettingsrmtautosetmodel.h \
    models/sprsettingsspectrumrangesmodel.h \
    tcp/TCPCommandSet.h \
    verticallabel.h \
    maintabwidget.h \
    sprsettingswidget.h \
    models/sprmainmodel.h \
    models/sprspectrumzonestablemodel.h \
    models/sprspectrumzonesmodel.h \
    models/sprspectrumitemmodel.h \
    models/sprspectrumlistitemsmodel.h \
    testtablewidget.h \
    sprspectrumgraphicswidget.h \
    sprtestimswidget.h \
    tcp/ServerConnect.h \
    tcp/ITCPCommand.h \
    tcp/TCPCommand.h \
    tcp/TCPTimeOutCommand.h \
    tcp/tcptimeoutwigget.h \
    tcp/tcplogswigtets.h \
    tcp/tcpcommandrguup.h \
    tcp/tcpcommandrguupdown.h \
    tcp/tcpcommandrentgeron.h \
    tcp/tcpcommandgetspectrums.h \
    tcp/tcpstartsoptestimscommand.h \
    tcp/tcpcommandstartpitatel.h \
    tcp/tcpstartrudospuskcommand.h \
    tcp/tcpstoprudostuskcommand.h \
    tcp/tcpcommandstoppitatel.h \
    models/sprseparatemodel.h \
    sprseparatewidget.h \
    tcp/tcpstartstopseparate.h \
    tcp/tcpgetspectrumsgistogramms.h \
    tcp/tcpgetrentgensparams.h \
    sprspectrumsmalltable.h \
    tcp/tcpteststartseparate.h \
    tcp/tcpteststopseparate.h \
    tcp/tcpseparatego.h \
    sprspectrumbaselisttable2.h \
    models/sprelementsmodel.h \
    variables/sprdatetimevariable.h \
    variables/sprenumvariable.h \
    variables/sprqcolorvariable.h \
    variables/sprqstringvariable.h \
    variables/sprvariable.h \
    variables/sprkprvariable.h \
    variables/sprbooleanvariable.h \
    firstcollumn2.h \
    sprelementpropertydialog.h \
    sprzonesranges.h \
    sprzonesrangestable.h \
    sprzonesrangestablewidget.h \
    sprspectrumlisttable.h \
    sprspectrumlisttablewidget.h \
    sprspectrumchoisetimedialog.h \
    sprdeucodestable.h \
    tcp/tcpcommandrentgenonfull.h \
    tcp/tcpcommandseparatoronoff.h \
    models/imainmodel.h \
    sprrentgenvatable.h

FORMS += \
    sprsettingsrentgenmainwidget.ui \
    sprsettingsrentgencontrolwidget.ui \
    sprsettingsrentgenautosetwidget.ui \
    sprformulaitemwidget.ui \
    sprsettingsformulawidget.ui \
    sprporogswidget.ui \
    sprsettingsporogswidget.ui \
    sprsettingsmainwidget.ui \
    sprsettingsrentgenwidget.ui \
    sprsettingsimswidget.ui \
    sprsettingscontrolwidget.ui \
    maintabwidget.ui \
    sprsettingswidget.ui \
    testtablewidget.ui \
    sprspectrumgraphicswidget.ui \
    sprtestimswidget.ui \
    tcp/tcptimeoutwigget.ui \
    tcp/tcplogswigtets.ui \
    sprseparatewidget.ui \
    firstcollumn2.ui \
    sprelementpropertydialog.ui \
    sprzonesrangestablewidget.ui \
    sprspectrumlisttablewidget.ui \
    sprspectrumchoisetimedialog.ui

RESOURCES += \
    icons.qrc \
    defxmlsettings.qrc

#DISTFILES += \
#    models.pri

#include(models/models.pri)
#include(tcp/tcp.pri)


unix:INCLUDEPATH += /usr/local/qwt-6.1.3/include
unix:LIBS += -L/usr/local/qwt-6.1.3/lib
#unix:LIBS += -l/usr/local/qwt-6.1.3/lib/libqwt.so

#unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += qwt

unix:!macx: LIBS += -L$$PWD/../../../../../../usr/local/qwt-6.1.3/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../../../../usr/local/qwt-6.1.3/include
DEPENDPATH += $$PWD/../../../../../../usr/local/qwt-6.1.3/include
