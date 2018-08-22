#include "sprpitatelcontrolwidget.h"

void SPRPitatelControlWidget::widgetsShow()
{
}

void SPRPitatelControlWidget::setFullMode(bool value)
{
    fullMode = value;
    if(separatorOn){
        separatorOn->setFullMode(value);
    }
}

ISPRModelData *SPRPitatelControlWidget::getModelData()
{
    return mainModel;
}

