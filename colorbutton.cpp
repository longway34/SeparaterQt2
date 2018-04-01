#include "colorbutton.h"

ColorButton::ColorButton(QWidget *parent): QPushButton(parent), color(Qt::red)
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(onClickMouse(bool)));
}

ColorButton::ColorButton(QColor color, QWidget *parent): QPushButton(parent)
{
    setColor(color);
    connect(this, SIGNAL(clicked(bool)), this, SLOT(onClickMouse(bool)));
}
