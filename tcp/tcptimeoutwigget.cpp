#include "tcp/tcptimeoutwigget.h"

TCPTimeOutWigget::TCPTimeOutWigget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
}

void TCPTimeOutWigget::onRangeChange(int step, int all)
{
    ui.thRange->setScale(0, double(all));
    ui.thRange->setValue(double(step));
    if(step == all){
        this->close();
    }
}
