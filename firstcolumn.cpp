#include "firstcolumn.h"
#include <QStyle>

FirstColumn::FirstColumn(QWidget *parent) :
    QWidget(parent), num(0), color(QColor(Qt::red))
{
    ui.setupUi(this);

    setColor(color);
    connect(ui.bColor, SIGNAL(changeColor(QColor)), this, SLOT(onColorClick(QColor)));
    connect(ui.bDelete, SIGNAL(clicked(bool)), this, SLOT(onDeleteClick(bool)));
    connect(ui.cbSelect, SIGNAL(toggled(bool)), this, SLOT(onSelectClick(bool)));
}


void FirstColumn::widgetsShow()
{
    setColor(color);
    setNumber(num);
}

