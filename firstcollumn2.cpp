#include "firstcollumn2.h"



void FirstCollumn2::setDeleteVisible(bool value)
{
    deleteVisible = value;
}

int FirstCollumn2::getRow() const
{
    return row;
}

void FirstCollumn2::setParams(int _row, QString _text, QColor _color, bool _selected){
    row = _row, text = _text, color = _color, ui.cbSelect->setChecked(_selected);
}

void FirstCollumn2::setRow(int value)
{
    row = value;
}

void FirstCollumn2::setSelectVisible(bool value)
{
    selectVisible = value;
}
