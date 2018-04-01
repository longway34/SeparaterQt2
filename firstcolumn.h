#ifndef FIRSTCOLUMN_H
#define FIRSTCOLUMN_H

#include "ui_firstcolumn.h"
#include <QColorDialog>
#include "isprwidget.h"

class FirstColumn : public QWidget, public ISPRWidget
{
    Q_OBJECT

    QColor color;
    int num;
public:
    explicit FirstColumn(QWidget *parent = 0);

    Ui::FirstColumn ui;
    QColor getMyColor(){return color;}
    int getMyNum(){return num;}
    void setData(int num, QColor(color)){
        setNumber(num);
        setColor(color);
    }

    void setColor(QColor _color){
        color = _color;
        ui.bColor->setColor(color);
        setNumber(ui.cbSelect->text().toInt());

    }
    void setNumber(int num){
        this->num = num;
        QPalette pal = ui.cbSelect->palette();
        pal.setColor(ui.cbSelect->foregroundRole(), this->getMyColor());
        ui.cbSelect->setPalette(pal);
        ui.cbSelect->setText(QString::number(this->num));
        ui.cbSelect->update();
    }
    bool isRowChecked(){return ui.cbSelect->isChecked();}
public slots:
    void onColorClick(QColor color){
        setColor(color);
        emit changeColor(color);
    }
    void onDeleteClick(bool){
        emit iAmDelete(num);
    }
    void onSelectClick(bool val){
        emit iAmSelected(num);
    }
    virtual void widgetsShow();
signals:
    void iAmSelected(int);
    void iAmDelete(int);
    void changeColor(QColor);
private:

    // ISPRWidget interface
public:
    virtual ISPRModelData *getModel(){return nullptr;}
};

#endif // FIRSTCOLUMN_H
