#ifndef TYPES_H
#define TYPES_H

#include <QMetaType>
#include <QtGlobal>
#include <QColor>
#include <QMap>
#include <QString>
#include <QVector>
#include <QDateTime>

//#include "models/sprvariable.h"
//#include "models/sprelementvariable.h"

// comment 02.04.2018

typedef enum eTypePRAM : int{
    pram50 = 0,
    pram50mc,
    module50
} TypePRAM;

Q_DECLARE_METATYPE(TypePRAM)

typedef enum eTypeTermo : int{
    hotModule = 0,
    coldModule
} TypeThermo;

Q_DECLARE_METATYPE(TypeThermo)

#define VERSION "2.0"

#define DEF_SPR_MAIN_NAME  "Сепаратор"
#define DEF_SPR_MAIN_ADDRESS "192.169.0.50"
#define DEF_SPR_MAIN_PORT 3425
#define DEF_SPR_MAIN_THREADS   4
#define DEF_SPR_MAIN_THREADS_S   "4"
#define MAX_SPR_MAIN_THREADS   4
#define DEF_SPR_MAIN_RENTGENS  2
#define DEF_SPR_MAIN_RENTGENS_S  "2"
#define MAX_SPR_MAIN_RENTGENS  2
#define DEF_SPR_MAIN_IMS       4
#define DEF_SPR_MAIN_IMS_S       "4"
#define MAX_SPR_MAIN_IMS       8
#define DEF_SPR_MAIN_IMS_ROW_COUNT       1
#define DEF_SPR_MAIN_IMS_ROW_COUNT_S       "1"
#define MAX_SPR_MAIN_IMS_ROW_COUNT       2
#define DEF_SPR_MAIN_TYPE_THERMO  (hotModule)
#define DEF_SPR_MAIN_TYPE_PRAM    (pram50)
#define DEF_SPR_MAIN_COLOR    (QColor(Qt::blue))
#define DEF_SPR_MAIN_DATE (QDate::currentDate())
#define DEF_SPR_MAIN_VERSION  VERSION
#define DEF_SPR_MAIN_SPECTRUM_FNAME_SUFFIX (QObject::tr(".spc"))
#define DEF_SPR_MAIN_SETTINGS_FNAME (QObject::tr("Separator"))
#define DEF_SPR_MAIN_SETTINGS_FNAME_SUFFIX (QObject::tr(".xml"))
\

#define SPR_SETTINGS_MAIN_NAME_XPATH  "SEPARATOR[NAME]"
#define SPR_SETTINGS_MAIN_ADDRESS_XPATH  "SEPARATOR[IP]"
#define SPR_SETTINGS_MAIN_PORT_XPATH  "SEPARATOR[PORT]"
#define SPR_SETTINGS_MAIN_THREADS_XPATH  "SEPARATOR[CHANNELS]"
#define SPR_SETTINGS_MAIN_RENTGENS_XPATH  "SEPARATOR[TUBES]"
#define SPR_SETTINGS_MAIN_IMS_XPATH  "SEPARATOR[IM_COUNT]"
#define SPR_SETTINGS_MAIN_IMS_ROW_COUNT_XPATH  "SEPARATOR[IM_ROW_COUNT]"
#define SPR_SETTINGS_MAIN_COLOR_XPATH  "SEPARATOR[COLOR]"
#define SPR_SETTINGS_MAIN_TYPE_PRAM_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=RMT]/VALUE[@NAME=ROENTGENS_TYPE]"
#define SPR_SETTINGS_MAIN_TYPE_THERMO_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=TERMO_ELEMENT]/VALUE[@NAME=MODE]"
#define SPR_SETTINGS_MAIN_DATE_XPATH  "CONFIG[DATE]"
#define SPR_SETTINGS_MAIN_VERSION_XPATH  "CONFIG[VERSION]"
#define SPR_SETTINGS_MAIN_SPECTRUM_FNAME_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=SPECTRUMS]/VALUE[@NAME=FILE_NAME]"

#define DEF_SPR_RENTGEN_I_TUBE    40
#define DEF_SPR_RENTGEN_U_TUBE    40
#define DEF_SPR_RENTGEN_DEU_CODE    165
#define DEF_SPR_RENTGEN_CP_CODE    120
#define DEF_SPR_RENTGEN_TIME_ON_RA    5
#define DEF_SPR_RENTGEN_TIME_OFF_RA    5
#define DEF_SPR_RENTGEN_TIME_HOT_RA    5
#define DEF_SPR_RENTGEN_WO_CONTROL_PLACE    false
#define DEF_SPR_RENTGEN_DIFF_PARAM_ALLOW    5
#define DEF_SPR_RENTGEN_DIFF_PARAM_DISALLOW   10
#define DEF_SPR_RENTGEN_TIME_MOVE_RGU     35

#define SPR_RENTGEN_I_TUBE_0_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=PRAM_TUBE_0]/VALUE[@NAME=I]"
#define SPR_RENTGEN_I_TUBE_1_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=PRAM_TUBE_1]/VALUE[@NAME=I]"
#define SPR_RENTGEN_U_TUBE_0_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=PRAM_TUBE_0]/VALUE[@NAME=U]"
#define SPR_RENTGEN_U_TUBE_1_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=PRAM_TUBE_1]/VALUE[@NAME=U]"

#define SPR_RENTGEN_DEU_CODE_0_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=CHANNEL_0]/VALUE[@NAME=DEU]"
#define SPR_RENTGEN_DEU_CODE_1_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=CHANNEL_1]/VALUE[@NAME=DEU]"
#define SPR_RENTGEN_DEU_CODE_2_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=CHANNEL_2]/VALUE[@NAME=DEU]"
#define SPR_RENTGEN_DEU_CODE_3_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=CHANNEL_3]/VALUE[@NAME=DEU]"
#define SPR_RENTGEN_CP_CODE_0_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=CHANNEL_0]/VALUE[@NAME=CP]"
#define SPR_RENTGEN_CP_CODE_1_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=CHANNEL_1]/VALUE[@NAME=CP]"
#define SPR_RENTGEN_CP_CODE_2_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=CHANNEL_2]/VALUE[@NAME=CP]"
#define SPR_RENTGEN_CP_CODE_3_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=CHANNEL_3]/VALUE[@NAME=CP]"

#define SPR_RENTGEN_TIME_ON_RA_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=RMT]/VALUE[@NAME=XRAY_TIME_ON]"
#define SPR_RENTGEN_TIME_OFF_RA_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=RMT]/VALUE[@NAME=XRAY_TIME_OFF]"
#define SPR_RENTGEN_TIME_HOT_RA_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=RMT]/VALUE[@NAME=XRAY_TIME_EXPOSITION]"

#define SPR_RENTGEN_WO_CONTROL_PLACE "SEPARATOR/PROPERTIES/PROPERTY[@CODE=RMT]/VALUE[@NAME=USE_OLD_RGU_COMMAND]"
#define SPR_RENTGEN_TIME_MOVE_RGU "SEPARATOR/PROPERTIES/PROPERTY[@CODE=RMT]/VALUE[@NAME=RGU_UP_DOWN_TIME]"

#define SPR_RENTGEN_DIFF_PARAM_ALLOW "SEPARATOR/PROPERTIES/PROPERTY[@CODE=RMT]/VALUE[@NAME=ROENTGEN_DEVIATION_PERMISSIBLE]"
#define SPR_RENTGEN_DIFF_PARAM_DISALLOW "SEPARATOR/PROPERTIES/PROPERTY[@CODE=RMT]/VALUE[@NAME=ROENTGEN_DEVIATION_CRITICAL]"

#define MAX_SPR_SPECTOR_ELEMENTS  6
#define MAX_SPR_SPECTOR_CHANNELS    256

typedef enum : int{
    Ns = 0,
    Fe,
    Cu,
    Mo,
    Zn,
    Mg
} EnumElements;

Q_DECLARE_METATYPE(EnumElements)

typedef struct sElementsData
{
    QString sname;
    QString name;
    QColor color;
    qint16 index;
    qint16 min;
    qint16 max;
} DefaultElementsProperty;

typedef QMap<EnumElements, DefaultElementsProperty> DefaultMapElements;

Q_DECLARE_METATYPE(DefaultMapElements)

static const DefaultMapElements DEF_SPR_FORMULA_ELEMENTS_PROPERTY = {
    {Ns, {"Ns", QString("Рассеенка"), QColor(255,0,0,100), 0, 80, 256}},
    {Fe, {"Fe", QString("Железо"), QColor(255, 0, 0, 100), 1, 20, 40}},
    {Cu, {"Cu", QString("Медь"), QColor(0,255,0,100), 2, 40, 60}},
    {Mo, {"Mo", QString("Молибден"), QColor(0,0,255,100), 3, 60, 80}},
    {Zn, {"Zn", QString("Цинк"), QColor(255,255,0,100), 4, 80, 100}},
    {Mg, {"Mg", QString("Магний"), QColor(0,255,255,100), 5, 100, 120}},
};

#define SPR_SETTINGS_SPECTRUM_ZONES_XPATH_PREFIX "SEPARATOR/PROPERTY[@CODE=RANGES_NAMES]/VALUE[@NAME=NAME"
#define SPR_SETTINGS_SPECTRUM_ZONES_KEY_XPATH_SUFFIX   "[KEY]"
#define SPR_SETTINGS_SPECTRUM_ZONES_FNAME_XPATH_SUFFIX "[FNAME]"
#define SPR_SETTINGS_SPECTRUM_ZONES_COLOR_XPATH_SUFFIX "[COLOR]"

typedef enum:int{
    H1  =0,
    H2,
    H3
} TypeConditions;

Q_DECLARE_METATYPE(TypeConditions)

typedef QMap<TypeConditions, int> MapConditions;
static const MapConditions mapConditions = {
    {H1, 1},
    {H2, 4},
    {H3, 6}
};


#define MAX_SPR_FORMULA_NUMBER   3
#define DEF_SPR_FORMULA_NUMBER   3

#define DEF_SPR_FORMULA_ELEMENT (Ns)
#define DEF_SPR_FORMULA_MUL 0
#define DEF_SPR_FORMULA_MIN 0
#define DEF_SPR_FORMULA_MAX 2

#define SPR_FORMULA_ELEMENT_UP_1_1_XPATH "SEPARATOR/SEPARATE_SETUP/SH1[s0]"
#define SPR_FORMULA_ELEMENT_UP_1_2_XPATH "SEPARATOR/SEPARATE_SETUP/SH2[s0]"
#define SPR_FORMULA_ELEMENT_UP_1_3_XPATH "SEPARATOR/SEPARATE_SETUP/SH3[s0]"
#define SPR_FORMULA_ELEMENT_UP_2_1_XPATH "SEPARATOR/SEPARATE_SETUP/SH1[s1]"
#define SPR_FORMULA_ELEMENT_UP_2_2_XPATH "SEPARATOR/SEPARATE_SETUP/SH2[s1]"
#define SPR_FORMULA_ELEMENT_UP_2_3_XPATH "SEPARATOR/SEPARATE_SETUP/SH3[s1]"

#define SPR_FORMULA_ELEMENT_DOWN_1_1_XPATH "SEPARATOR/SEPARATE_SETUP/SH1[s2]"
#define SPR_FORMULA_ELEMENT_DOWN_1_2_XPATH "SEPARATOR/SEPARATE_SETUP/SH2[s2]"
#define SPR_FORMULA_ELEMENT_DOWN_1_3_XPATH "SEPARATOR/SEPARATE_SETUP/SH3[s2]"
#define SPR_FORMULA_ELEMENT_DOWN_2_1_XPATH "SEPARATOR/SEPARATE_SETUP/SH1[s3]"
#define SPR_FORMULA_ELEMENT_DOWN_2_2_XPATH "SEPARATOR/SEPARATE_SETUP/SH2[s3]"
#define SPR_FORMULA_ELEMENT_DOWN_2_3_XPATH "SEPARATOR/SEPARATE_SETUP/SH3[s3]"

#define SPR_FORMULA_ELEMENT_DOWN_3_1_XPATH "SEPARATOR/SEPARATE_SETUP/SH1[s4]"
#define SPR_FORMULA_ELEMENT_DOWN_3_2_XPATH "SEPARATOR/SEPARATE_SETUP/SH2[s4]"
#define SPR_FORMULA_ELEMENT_DOWN_3_3_XPATH "SEPARATOR/SEPARATE_SETUP/SH3[s4]"
#define SPR_FORMULA_ELEMENT_DOWN_4_1_XPATH "SEPARATOR/SEPARATE_SETUP/SH1[s5]"
#define SPR_FORMULA_ELEMENT_DOWN_4_2_XPATH "SEPARATOR/SEPARATE_SETUP/SH2[s5]"
#define SPR_FORMULA_ELEMENT_DOWN_4_3_XPATH "SEPARATOR/SEPARATE_SETUP/SH3[s5]"

#define SPR_FORMULA_MUL_UP_1_XPATH    "SEPARATOR/SEPARATE_SETUP/KH1[k0]"
#define SPR_FORMULA_MUL_UP_2_XPATH    "SEPARATOR/SEPARATE_SETUP/KH2[k0]"
#define SPR_FORMULA_MUL_UP_3_XPATH    "SEPARATOR/SEPARATE_SETUP/KH3[k0]"
#define SPR_FORMULA_MUL_DOWN_1_XPATH  "SEPARATOR/SEPARATE_SETUP/KH1[k1]"
#define SPR_FORMULA_MUL_DOWN_2_XPATH  "SEPARATOR/SEPARATE_SETUP/KH2[k1]"
#define SPR_FORMULA_MUL_DOWN_3_XPATH  "SEPARATOR/SEPARATE_SETUP/KH3[k1]"

#define SPR_FORMULA_MIN_XPATH     "SEPARATOR/SEPARATE_SETUP/MinG1[Value]"
#define SPR_FORMULA_MAX_XPATH     "SEPARATOR/SEPARATE_SETUP/MaxG1[Value]"

#define DEF_SPR_FORMULA_CONDITION   (H1)
#define MAX_SPR_FORMULA_CONDITION   6

typedef enum:int{
    OnTail = 0,
    OnConsentrate
} TypeSelection;

Q_DECLARE_METATYPE(TypeSelection)

#define DEF_SPR_POROGS_FOR_MIN_STONE    1
#define DEF_SPR_POROGS_FOR_MAX_STONE    1
#define DEF_SPR_POROGS_X_RAY_CORRECTION 2
#define DEF_SPR_POROGS_TYPE_SELECTION   (OnConsentrate)
#define DEF_SPR_POROGS_INVERT_SELECTION (false)

#define SPR_POROGS_CONDITION_XPATH  "SEPARATOR/SEPARATE_SETUP/fH12[Value]"
#define SPR_POROGS_FOR_MIN_STONE_XPATH "SEPARATOR/SEPARATE_SETUP/K_PR_MIN[Value]"
#define SPR_POROGS_FOR_MAX_STONE_XPATH "SEPARATOR/SEPARATE_SETUP/K_PR_MAX[Value]"
#define SPR_POROGS_X_RAY_CORRECTION_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=SEPARATION]/VALUE[@NAME=X_RAY_CORRECTION]"
#define SPR_POROGS_TYPE_SELECTION_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=SEPARATION]/VALUE[@NAME=SELECTION_IS_CONCENTRATE]"
#define SPR_POROGS_INVERT_SELECTION "SEPARATOR/SEPARATE_SETUP/fOtb[Value]"
#define SPR_POROGS_INVERT_SELECTION2 "SEPARATOR/SEPARATE_SETUP/fOtbR2[Value]"

typedef struct {
    QObject *sender;
    qint8 data_int8;
    qint16 data_int16;
    bool data_bool;
    double data_double;
    QString data_string;
    struct
    {
        quint16 row;
        quint16 col;

    } pointInt;
    QString toPrint(){
        return "int8: "+QString::number(data_int8)+
                "\n int16: "+QString::number(data_int16)+
                "\n bool: "+ QString((data_bool == true) ? "true" : "false")+
                "\n double: "+QString::number(data_double)+
                "\n str: "+data_string+
                "\n row: "+QString::number(pointInt.row)+
                "\n col: "+QString::number(pointInt.col);
    }
} viewChangeData;

typedef enum {
    SettingsMain = 0,
    SimplePorogs,
    SettingsPorogs,
    SettingsRentgen,
    SettingFormula,
    FormulaItem,
    AutosetWidget
} WidgetsEnum;

typedef enum {
    SettingsMainModel = 0,
    SimplePorogsModel,
    SettingsPorogsModel,
    SettingsRentgenModel,
    SettingFormulaModel,
    FormulaItemModel,
    AutosetModel
} ModelsEnum;

typedef enum:int {
    OnCP_Only = 0,
    OnDEU_CP
} TypeAutoSettingRMT;

Q_DECLARE_METATYPE(TypeAutoSettingRMT)

#define DEF_SPR_AUTOSETTINGS_TYPE_SETTING   (OnDEU_CP)
#define DEF_SPR_AUTOSETTINGS_CODE_BEGIN 120
#define DEF_SPR_AUTOSETTINGS_CODE_STEP 20
#define DEF_SPR_AUTOSETTINGS_WITH_RGU (true)
#define DEF_SPR_AUTOSETTINGS_PEAC_POSITION  64
#define DEF_SPR_AUTUSETTINGS_WITH_CHANNEL   (true)

// ??????? #define SPR_AUTOSETTING_TYPE_XPATH  "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AUTO_CODE_SET]/VALUE[@NAME="
#define SPR_AUTOSETTINGS_TYPE_SETTING_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AUTO_CODE_SET]/VALUE[@NAME=USE_DEU]"
#define SPR_AUTOSETTINGS_CODE_BEGIN_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AUTO_CODE_SET]/VALUE[@NAME=START_DEU]"
#define SPR_AUTOSETTINGS_CODE_STEP_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AUTO_CODE_SET]/VALUE[@NAME=STEP_DEU]"
#define SPR_AUTOSETTINGS_WITH_RGU_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AUTO_CODE_SET]/VALUE[@NAME=USE_RGU]"
#define SPR_AUTOSETTINGS_PEAC_POSITION_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AUTO_CODE_SET]/VALUE[@NAME=PEAK_POSITION]"
#define SPR_AUTUSETTINGS_WITH_CHANNEL_0 "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AUTO_CODE_SET]/VALUE[@NAME=USE_CHANNEL_0]"
#define SPR_AUTUSETTINGS_WITH_CHANNEL_1 "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AUTO_CODE_SET]/VALUE[@NAME=USE_CHANNEL_1]"
#define SPR_AUTUSETTINGS_WITH_CHANNEL_2 "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AUTO_CODE_SET]/VALUE[@NAME=USE_CHANNEL_2]"
#define SPR_AUTUSETTINGS_WITH_CHANNEL_3 "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AUTO_CODE_SET]/VALUE[@NAME=USE_CHANNEL_3]"
#define SPR_AUTUSETTINGS_WITH_CHANNEL_4 "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AUTO_CODE_SET]/VALUE[@NAME=USE_CHANNEL_4]"
#define SPR_AUTUSETTINGS_WITH_CHANNEL_5 "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AUTO_CODE_SET]/VALUE[@NAME=USE_CHANNEL_5]"
#define SPR_AUTUSETTINGS_WITH_CHANNEL_6 "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AUTO_CODE_SET]/VALUE[@NAME=USE_CHANNEL_6]"
#define SPR_AUTUSETTINGS_WITH_CHANNEL_7 "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AUTO_CODE_SET]/VALUE[@NAME=USE_CHANNEL_7]"


//typedef QVector<MapElements> SeparatorSettingsSpectorData;
typedef QMap<quint16, DefaultMapElements> SeparatorSettingsSpectorModel;

typedef QVector<double> SeparatorPorogsValuesOnThread;

//Q_DECLARE_METATYPE(SeparatorPorogsValuesOnThread)

typedef QVector<SeparatorPorogsValuesOnThread> SeparatorPorogsValues;

typedef struct{
    quint16 threads;
    quint16 conditions;
    SeparatorPorogsValues values;
} SeparatorPorogsData;

#define DEF_SPR_IMS_T_METTERING_MIN_STONE   16
#define DEF_SPR_IMS_T_METTERING_MAX_STONE   100
#define DEF_SPR_IMS_T_METTERING_MAX_NAX_STONE   0
#define DEF_SPR_IMS_T_DELAY_MIN_STONE   150
#define DEF_SPR_IMS_T_DELAY_MAX_STONE   100
#define DEF_SPR_IMS_T_DELAY_MAX_MAX_STONE   0
#define DEF_SPR_IMS_T_DURATION_MIN_STONE   40
#define DEF_SPR_IMS_T_DURATION_MAX_STONE   80
#define DEF_SPR_IMS_T_DURATION_MAX_MAX_STONE   0

#define DEF_SPR_IMS_BLOCK_PARAM   8

#define DEF_SPR_IMS_SPEAD_ORE_A   -5.2
#define DEF_SPR_IMS_SPEAD_ORE_B   4.522

#define DEF_SPR_IMS_LIMIT_ABRASION   300000
#define DEF_SPR_IMS_ABRASION   0
#define DEF_SPR_IMS_WITH_MAX_STONE  (false)
#define DEF_SPR_IMS_PARTS_SIZE  4


#define SPR_IMS_T_METTERING_MIN_STONE_XPATH   "SEPARATOR/PROPERTIES/PROPERTY[@CODE=IM]/VALUE[@NAME=MIN_SIZE]"
#define SPR_IMS_T_METTERING_MAX_STONE_XPATH   "SEPARATOR/PROPERTIES/PROPERTY[@CODE=IM]/VALUE[@NAME=MAX_SIZE]"
#define SPR_IMS_T_METTERING_MAX_MAX_STONE_XPATH   "SEPARATOR/PROPERTIES/PROPERTY[@CODE=MAX_STONE_PARAMETERS]/VALUE[@NAME=MEASURE0]"
#define SPR_IMS_T_DELAY_MIN_STONE_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=IM]/VALUE[@NAME=MIN_PAUSE]"
#define SPR_IMS_T_DELAY_MAX_STONE_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=IM]/VALUE[@NAME=MAX_PAUSE]"
#define SPR_IMS_T_DELAY_MAX_MAX_STONE_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=MAX_STONE_PARAMETERS]/VALUE[@NAME=DELAY0]"
#define SPR_IMS_T_DURATION_MIN_STONE_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=IM]/VALUE[@NAME=MIN_DURATION]"
#define SPR_IMS_T_DURATION_MAX_STONE_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=IM]/VALUE[@NAME=MAX_DURATION]"
#define SPR_IMS_T_DURATION_MAX_MAX_STONE_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=MAX_STONE_PARAMETERS]/VALUE[@NAME=DURATION0]"
#define SPR_IMS_WITH_MAX_STONE "SEPARATOR/PROPERTIES/PROPERTY[@CODE=MAX_STONE_PARAMETERS]/VALUE[@NAME=USE_MAX_STONE_PARAMETERS]"

#define SPR_IMS_BLOCK_PARAM_XPATH "SEPARATOR/SEPARATE_SETUP/TOtb[Value]"
#define SPR_IMS_BLOCK_PARAM2_XPATH "SEPARATOR/SEPARATE_SETUP/TOtbR2[Value]"

#define SPR_IMS_SPEAD_ORE_A "SEPARATOR/PROPERTIES/PROPERTY[@CODE=ORE_SPEED_KOEF]/VALUE[@NAME=KOEF_A]"
#define SPR_IMS_SPEAD_ORE_B "SEPARATOR/PROPERTIES/PROPERTY[@CODE=ORE_SPEED_KOEF]/VALUE[@NAME=KOEF_B]"

#define SPR_IMS_LIMIT_ABRASION_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=IM]/VALUE[@NAME=LIMIT_ABRASION]"
#define SPR_IMS_ABRASIONS_XPATH_PREFIX "SEPARATOR/PROPERTIES/PROPERTY[@CODE=IM]/VALUE[@NAME=ABRASION"

#define SPR_IMS_K_DELAY_XPATH_PREFIX "SEPARATOR/SEPARATE_SETUP/K_zd[u"
#define SPR_IMS_B_DELAY_XPATH_PREFIX "SEPARATOR/SEPARATE_SETUP/B_zd[u"
#define SPR_IMS_K_DURATION_XPATH_PREFIX "SEPARATOR/SEPARATE_SETUP/K_im[u"
#define SPR_IMS_B_DURATION_XPATH_PREFIX "SEPARATOR/SEPARATE_SETUP/B_im[u"

#define SPR_IMS_TIMES_METTERING_XPATH_PREFIX "SEPARATOR/SEPARATE_SETUP/Tiz[t"

#define SPR_CONTROL_CORREL_SPECTRUM_PERMISS_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=CORRELATION_CONTROL_SPECTRUM]/VALUE[@NAME=PERMISSIBLE]"
#define SPR_CONTROL_CORREL_SPECTRUM_CRITICAL_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=CORRELATION_CONTROL_SPECTRUM]/VALUE[@NAME=CRITICAL]"
#define SPR_CONTROL_SPEED_STREAM_PERMISS_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=SPEED_STREAM]/VALUE[@NAME=PERMISSIBLE]"
#define SPR_CONTROL_SPEED_STREAM_CRITICAL_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=SPEED_STREAM]/VALUE[@NAME=CRITICAL]"
#define SPR_CONTROL_DIFF_CENTER_GRAVITY_PERMISS_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=CENTER_DEVIATION]/VALUE[@NAME=PERMISSIBLE]"
#define SPR_CONTROL_DIFF_CENTER_GRAVITY_CRITICAL_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=CENTER_DEVIATION]/VALUE[@NAME=CRITICAL]"
#define SPR_CONTROL_AIR_LOADING_MIN_PERMISS_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AIR_LOADING]/VALUE[@NAME=PERMISSIBLE_MIN]"
#define SPR_CONTROL_AIR_LOADING_MIN_CRITICAL_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AIR_LOADING]/VALUE[@NAME=CRITICAL_MIN]"
#define SPR_CONTROL_AIR_LOADING_MAX_PERMISS_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AIR_LOADING]/VALUE[@NAME=PERMISSIBLE_MAX]"
#define SPR_CONTROL_AIR_LOADING_MAX_CRITICAL_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=AIR_LOADING]/VALUE[@NAME=CRITICAL_MAX]"
#define SPR_CONTROL_CONTROL_AREA_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=CONTROL_RANGE]/VALUE[@NAME=INDEX]"
#define SPR_CONTROL_WEIGHT_ARG_STONE_TAIL_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=STONE_DENSITY]/VALUE[@NAME=TAIL]"
#define SPR_CONTROL_WEIGHT_ARG_STONE_CONCENTRATE_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=STONE_DENSITY]/VALUE[@NAME=CONCENTRATE]"
#define SPR_CONTROL_T_MEASSURE_FOR_DATA_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=SEPARATION_TIME]/VALUE[@NAME=TIME_FOR_DATA]"
#define SPR_CONTROL_T_MEASSURE_FOR_SPECTRUM_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=SEPARATION_TIME]/VALUE[@NAME=TIME_FOR_SPECTRUM]"
#define SPR_CONTROL_T_MEASSURE_FOR_HISTOGRAMM_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=SEPARATION_TIME]/VALUE[@NAME=TIME_FOR_HISTOGRAMM]"
#define SPR_CONTROL_CORRECT_OPTIMAL_ORE_STREAM_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=LEVEL_SELECTION]/VALUE[@NAME=CORRECTION]"
#define SPR_CONTROL_AUTO_STREAM_CONTROL_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=LEVEL_SELECTION]/VALUE[@NAME=ON]"
#define SPR_CONTROL_VEMS_LEVEL_LESS_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=PITATEL]/VALUE[@NAME=DEC_CODE_PERCENT]"
#define SPR_CONTROL_VEMS_MAX_CODE_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=PITATEL]/VALUE[@NAME=MAX_CODE_PERCENT]"
#define SPR_CONTROL_VEMS_BEGIN_CODE_XPATH "SEPARATOR/PROPERTIES/PROPERTY[@CODE=PITATEL]/VALUE[@NAME=CODE]"

#define DEF_SPR_CONTROL_CORREL_SPECTRUM_PERMISS 0.7
#define DEF_SPR_CONTROL_CORREL_SPECTRUM_CRITICAL 0.6
#define DEF_SPR_CONTROL_SPEED_STREAM_PERMISS    8
#define DEF_SPR_CONTROL_SPEED_STREAM_CRITICAL   10
#define DEF_SPR_CONTROL_DIFF_CENTER_GRAVITY_PERMISS     3
#define DEF_SPR_CONTROL_DIFF_CENTER_GRAVITY_CRITICAL    5
#define DEF_SPR_CONTROL_AIR_LOADING_MIN_PERMISS         1000
#define DEF_SPR_CONTROL_AIR_LOADING_MIN_CRITICAL        500
#define DEF_SPR_CONTROL_AIR_LOADING_MAX_PERMISS         3500
#define DEF_SPR_CONTROL_AIR_LOADING_MAX_CRITICAL        5000
#define DEF_SPR_CONTROL_CONTROL_AREA                (Ns)
#define DEF_SPR_CONTROL_WEIGHT_ARG_STONE_TAIL           400
#define DEF_SPR_CONTROL_WEIGHT_ARG_STONE_CONCENTRATE    400
#define DEF_SPR_CONTROL_T_MEASSURE_FOR_DATA         5
#define DEF_SPR_CONTROL_T_MEASSURE_FOR_SPECTRUM     15
#define DEF_SPR_CONTROL_T_MEASSURE_FOR_HISTOGRAMM   25
#define DEF_SPR_CONTROL_CORRECT_OPTIMAL_ORE_STREAM      5
#define DEF_SPR_CONTROL_AUTO_STREAM_CONTROL     (true)
#define DEF_SPR_CONTROL_VEMS_LEVEL_LESS     5
#define DEF_SPR_CONTROL_VEMS_MAX_CODE       100
#define DEF_SPR_CONTROL_VEMS_BEGIN_CODE       880 // persent * 20 (44%)

#define DEF_SPECTRUM_DATA_BUF_LENGTH    708
#define DEF_SPECTRUM_DATA_LENGTH_BYTE 512
#define DEF_SPECTRUM_DATA_LENGTH (DEF_SPECTRUM_DATA_LENGTH_BYTE / 2)

#define SPR_SEPARATE_GMZ_PATH_PREFIX "SEPARATOR/SEPARATE_SETUP/GmZ[g"
#define SPR_SEPARATE_GCOL_PATH  "SEPARATOR/SEPARATE_SETUP/GCol[Value]"
#define SPR_SEPARATE_KRUCH_PATH  "SEPARATOR/SEPARATE_SETUP/KRuch[Value]"
#define SPR_SEPARATE_USL_PATH_PREFIX "SEPARATOR/SEPARATE_SETUP/Usl[u"
#define SPR_SEPARATE_ALG_PATH   "SEPARATOR/SEPARATE_SETUP/Alg[Value]"
#define SPR_SEPARATE_SEP_ROW_PATH  "SEPARATOR/SEPARATE_SETUP/Sep_Row[Value]"

#define DEF_SPR_SEPARATE_GMZ        25
#define DEF_SPR_SEPARATE_GCOL       200
#define DEF_SPR_SEPARATE_KRUCH      4
#define DEF_SPR_SEPARATE_USL        1
#define DEF_SPR_SEPARATE_ALG        0
#define DEF_SPR_SEPARATE_SEP_ROW    0

#endif // TYPES_H

