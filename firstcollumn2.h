#ifndef FIRSTCOLLUMN2_H
#define FIRSTCOLLUMN2_H

#include "ui_firstcollumn2.h"
#include <QColor>
#include <QStyleFactory>
#include <QColorDialog>

class FirstCollumn2 : public QWidget
{
    Q_OBJECT

    bool selectVisible;
    bool deleteVisible;
    bool textVisible;
    bool colorVisible;

    QString text;
    QColor color;
    int row;
public:

    explicit FirstCollumn2(QWidget *parent = 0) :
        QWidget(parent), selectVisible(true), deleteVisible(true), textVisible(true), colorVisible(true), text("99"), color(QColor(Qt::black)), row(-1)
    {
        ui.setupUi(this);

        connect(ui.bColor, SIGNAL(clicked(bool)), this, SLOT(onColorButtomClick(bool)));
        connect(ui.bDelete, SIGNAL(clicked(bool)), this, SLOT(onDeleteButtomClick(bool)));
        connect(ui.cbSelect, SIGNAL(toggled(bool)), this, SLOT(onSelectedChange(bool)));
    }

    void setColor(QColor _color){
        if(_color.isValid()){
            color = _color;
            widgetsShow();
        }
    }
    QColor getColor() {return color;}
    QString getText() {return text;}

    bool isSelect(){return ui.cbSelect->isChecked();}
    void setSelect(bool _value){ui.cbSelect->setChecked(_value);}

    void setText(QString _text){
        text = _text;
        widgetsShow();
    }
    void setSelectVisible(bool value);

    void setDeleteVisible(bool value);

    int getRow() const;
    void setRow(int value);

private:
    Ui::FirstCollumn2 ui;

public slots:
    void widgetsShow();
    void onColorButtomClick(bool){
        QColor newColor = QColorDialog::getColor(color, this);
        if(newColor.isValid() && newColor != color){
            setColor(newColor);
            emit colorChanged(newColor);
        }
    }

    void onDeleteButtomClick(bool){
        emit deleteRow(getRow());
    }

    void onSelectedChange(bool state){
        emit selectedRow(state, row);
    }

    void setParams(int _row, QString _text, QColor _color, bool _selected);
signals:
    void colorChanged(QColor);
    void deleteRow(int);
    void selectedRow(bool, int);
};

#endif // FIRSTCOLLUMN2_H
