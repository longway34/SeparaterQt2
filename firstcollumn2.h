#ifndef FIRSTCOLLUMN2_H
#define FIRSTCOLLUMN2_H

#include "ui_firstcollumn2.h"
#include <QColor>
#include <QStyleFactory>
#include <QColorDialog>

#include "variables/sprqcolorvariable.h"

class FirstCollumn2 : public QWidget
{
    Q_OBJECT

    bool selectVisible;
    bool deleteVisible;
    bool textVisible;
    bool colorVisible;

    QString textValue;
    QColor color;

    SPRQColorVariable *vcolor;

    int row;


public:

    explicit FirstCollumn2(QWidget *parent = 0);
    explicit FirstCollumn2(QString _textTitle, bool _colorVisible = true, QColor _color = QColor(Qt::white), bool _selectVisible = true, bool _deleteVisible = true, QWidget *parent = nullptr);

    void setColor(QColor _color);
    QColor getColor();
    QString getText() {return textValue;}

    bool isSelect(){return ui.cbSelect->isChecked();}
    void setSelect(bool _value){ui.cbSelect->setChecked(_value);}

    void setText(QString _textTitle);
    void setSelectVisible(bool value);

    void setDeleteVisible(bool value);

    int getRow() const;
    void setRow(int value);

    void setColorVariable(SPRQColorVariable *value);

    bool getColorVisible() const;
    void setColorVisible(bool value);

private:
    Ui::FirstCollumn2 ui;

public slots:

    virtual QSize sizeHint() const;

    void widgetsShow();
    void onColorButtomClick(bool);

    void onDeleteButtomClick(bool){
        emit deleteRow(getRow());
    }

    void onSelectedChange(bool state){
        emit selectedRow(state, row);
    }

    void setParams(int _row, QString _text, QColor _color, bool _selected);
    void onModelChanget(IModelVariable *);
signals:
    void colorChanged(QColor);
    void deleteRow(int);
    void selectedRow(bool, int);
};

#endif // FIRSTCOLLUMN2_H
