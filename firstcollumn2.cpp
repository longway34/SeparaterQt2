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

void FirstCollumn2::widgetsShow(){
    ui.cbSelect->setVisible(selectVisible);
    ui.bColor->setVisible(colorVisible);
    if(colorVisible){
        QString css_ = QWidget::styleSheet();
        QString css = QString("background-color: %1;").arg(color.name());
        //            QStyle *new_style = new QStyle(QStyleFactory::create("Fusion"));

        //            css += QString("border: %1;").arg("none");
        QPalette pal = ui.bColor->palette();
        pal.setColor(QPalette::Button, color);
        //            ui.bColor->setAutoFillBackground(true);
        //            ui.bColor->setStyleSheet(css);
        ui.bColor->setStyle(QStyleFactory::create("Fusion"));
        ui.bColor->setPalette(pal);
        ui.bColor->show();
    }
    ui.lText->setVisible(textVisible);
    if(textVisible){
        QPalette pal = ui.lText->palette();
        pal.setColor(ui.lText->foregroundRole(), color);
        ui.lText->setPalette(pal);
        ui.lText->setText(text);
    }
    ui.bDelete->setVisible(deleteVisible);
}

void FirstCollumn2::setColorVariable(SPRQColorVariable *value)
{
    if(value){
        if(vcolor != value){
            if(vcolor){
                disconnect(vcolor, SIGNAL(modelChanget(IModelVariable*)), this, SLOT(onModelChanget(IModelVariable*)));
            }
            vcolor = value;
        }

    }
}

void FirstCollumn2::onModelChanget(IModelVariable*){
    widgetsShow();
}

void FirstCollumn2::setSelectVisible(bool value)
{
    selectVisible = value;
}
