/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ITCPCommand.cpp
 * Author: longway
 * 
 * Created on 10 февраля 2018 г., 16:47
 */

#include "ITCPCommand.h"

TCPLogsWigtets *ITCPCommand::getLogWidget() const
{
    return logWidget;
}

void ITCPCommand::setLogWidget(TCPLogsWigtets *value)
{
    logWidget = value;
}

ITCPCommand::ITCPCommand(): logWidget(nullptr) {
}


ITCPCommand::~ITCPCommand() {
}

