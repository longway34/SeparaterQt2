#include "sprformulaitemmodel.h"

//SPRFormulaItemModel::SPRFormulaItemData *SPRFormulaItemModel::getModelData() const
//{
//    return modelData;
//}

//void SPRFormulaItemModel::setModelData(SPRFormulaItemData *value, int _index)
//{
//    modelData = value;
//    this->index = _index;
//}

void SPRFormulaItemModel::setMin(SPRVariable<double> *value)
{
    min = value;
    setProperty("delete_min", QVariant(false));
}

void SPRFormulaItemModel::setMax(SPRVariable<double> *value)
{
    max = value;
    setProperty("delete_max", QVariant(false));
}

SPRFormulaItemModel::SPRFormulaItemModel(QObject *parent):
    ISPRModelData()
{
}

SPRFormulaItemModel::SPRFormulaItemModel(QDomDocument *_doc, int _index, ISPRModelData *parent):
    ISPRModelData(_doc, parent), ElementUp1(nullptr), ElementUp2(nullptr),
        ElementDown1(nullptr), ElementDown2(nullptr), ElementDown3(nullptr), ElementDown4(nullptr),
        MulUp(nullptr), MulDown(nullptr), min(nullptr), max(nullptr)
{
    index = _index;
    QString xpath = "SEPARATOR/SEPARATE_SETUP/SH"+QString::number(index+1)+"[s0]";
    ElementUp1 = new SPRElementVariable(doc, xpath, DEF_SPR_FORMULA_ELEMENT, this);
    xpath = "SEPARATOR/SEPARATE_SETUP/SH"+QString::number(index+1)+"[s1]";
    ElementUp2 = new SPRElementVariable(doc, xpath, DEF_SPR_FORMULA_ELEMENT, this);

    xpath = "SEPARATOR/SEPARATE_SETUP/SH"+QString::number(index+1)+"[s2]";
    ElementDown1 = new SPRElementVariable(doc, xpath, DEF_SPR_FORMULA_ELEMENT, this);
    xpath = "SEPARATOR/SEPARATE_SETUP/SH"+QString::number(index+1)+"[s3]";
    ElementDown2 = new SPRElementVariable(doc, xpath, DEF_SPR_FORMULA_ELEMENT, this);
    xpath = "SEPARATOR/SEPARATE_SETUP/SH"+QString::number(index+1)+"[s4]";
    ElementDown3 = new SPRElementVariable(doc, xpath, DEF_SPR_FORMULA_ELEMENT, this);
    xpath = "SEPARATOR/SEPARATE_SETUP/SH"+QString::number(index+1)+"[s5]";
    ElementDown4 = new SPRElementVariable(doc, xpath, DEF_SPR_FORMULA_ELEMENT, this);

    xpath = "SEPARATOR/SEPARATE_SETUP/KH"+QString::number(index+1)+"[k0]";
    MulUp = new SPRVariable<double>(doc, xpath, DEF_SPR_FORMULA_MUL, this);
    xpath = "SEPARATOR/SEPARATE_SETUP/KH"+QString::number(index+1)+"[k1]";
    MulDown = new SPRVariable<double>(doc, xpath, DEF_SPR_FORMULA_MUL, this);

    max = new SPRVariable<double>(doc, SPR_FORMULA_MAX_XPATH, DEF_SPR_FORMULA_MAX, this);
    setProperty("delete_max", QVariant(true));
    min = new SPRVariable<double>(doc, SPR_FORMULA_MIN_XPATH, DEF_SPR_FORMULA_MIN, this);
    setProperty("delete_min", QVariant(true));
}

SPRFormulaItemModel::~SPRFormulaItemModel()
{
    if(ElementUp1) delete ElementUp1; ElementUp1 = nullptr;
    if(ElementUp2) delete ElementUp2; ElementUp2 = nullptr;
    if(ElementDown1) delete ElementDown1; ElementDown1 = nullptr;
    if(ElementDown2) delete ElementDown2; ElementDown2 = nullptr;
    if(ElementDown3) delete ElementDown3; ElementDown3 = nullptr;
    if(ElementDown4) delete ElementDown4; ElementDown4 = nullptr;

    if(MulUp) delete MulUp; MulUp = nullptr;
    if(MulDown) delete MulDown; MulDown = nullptr;

    if(min && property("delete_min").toBool()) delete min; min = nullptr;
    if(max && property("delete_max").toBool()) delete max; max = nullptr;
}
