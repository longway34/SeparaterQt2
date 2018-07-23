#include "sprsettingsrentgenautosetdialog.h"

SPRMainModel *SPRSettingsRentgenAutoSetDialog::getModelData()
{
    return mainModel;
}

void SPRSettingsRentgenAutoSetDialog::setModelData(SPRMainModel *value)
{
    mainModel = value;
}

void SPRSettingsRentgenAutoSetDialog::widgetsShow()
{

}

SPRSettingsRentgenAutoSetDialog::SPRSettingsRentgenAutoSetDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
}
