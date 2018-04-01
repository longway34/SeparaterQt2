#include "_types.h"

static const EnumElements elementsValue[MAX_SPR_SPECTOR_ELEMENTS] = {Ns, Fe, Cu, Mo, Zn, Mg};

static const DefaultElementsProperty elementsProperty[MAX_SPR_SPECTOR_ELEMENTS] = {
    {"Ns", "Рассеенка", QColor(255,0,255,100), 0, 80, 256},
    {"Fe", "Железо", QColor(255, 0, 0, 100), 1, 20, 40},
    {"Cu", "Медь", QColor(0,255,0,100), 40, 2, 60},
    {"Mo", "Молибден", QColor(0,0,255,100), 3, 60, 80},
    {"Zn", "Цинк", QColor(255,255,0,100), 4, 80, 100},
    {"Mg", "Магний", QColor(0,255,255,100), 5, 100, 120},

};

