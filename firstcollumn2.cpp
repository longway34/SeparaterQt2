#include "firstcollumn2.h"

void FirstCollumn2::setDeleteVisible(bool value)
{
    deleteVisible = value;
    ui.bDelete->setVisible(deleteVisible);
}

int FirstCollumn2::getRow() const
{
    return row;
}

void FirstCollumn2::setParams(int _row, QString _text, QColor _color, bool _selected){
    row = _row, setText(_text), color = _color, ui.cbSelect->setChecked(_selected);
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
        ui.lText->setText(textValue);
//        ui.lText->resize(ui.lText->fontMetrics().width(text), ui.lText->fontMetrics().height());
    }
    ui.bDelete->setVisible(deleteVisible);
    this->adjustSize();
}

void FirstCollumn2::onColorButtomClick(bool){
    QColor newColor = QColorDialog::getColor(color, this);
    if(newColor.isValid() && newColor != color){
        setColor(newColor);
        emit colorChanged(newColor);
    }
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

bool FirstCollumn2::getColorVisible() const
{
    return colorVisible;
}

void FirstCollumn2::setColorVisible(bool value)
{
    colorVisible = value;
}

QSize FirstCollumn2::sizeHint() const
{
    QSize res = QWidget::sizeHint();
    int s = layout()->spacing();
    int width = layout()->margin() *2 ;
//    QSize res = QSize(m, this->size().height());
    if(textVisible){
        int w = fontMetrics().width(textValue);
        width += ui.lText->sizeHint().width() + s;
    }
    if(colorVisible){
        width += ui.bColor->sizeHint().width() + s;
    }
    if(selectVisible){
        width += ui.cbSelect->sizeHint().width() + s;
    }
    if(deleteVisible){
        width += ui.bDelete->sizeHint().width() + s;
    }
    res = QSize(width, res.height());
    return res;
}

FirstCollumn2::FirstCollumn2(QWidget *parent) :
    QWidget(parent), vcolor(nullptr), selectVisible(true), deleteVisible(true), textVisible(true), colorVisible(true), color(QColor(Qt::black)), row(-1)
{
    ui.setupUi(this);

    connect(ui.bColor, SIGNAL(clicked(bool)), this, SLOT(onColorButtomClick(bool)));
    connect(ui.bDelete, SIGNAL(clicked(bool)), this, SLOT(onDeleteButtomClick(bool)));
    connect(ui.cbSelect, SIGNAL(toggled(bool)), this, SLOT(onSelectedChange(bool)));
}

FirstCollumn2::FirstCollumn2(QString _textTitle, bool _colorVisible, QColor _color, bool _selectVisible, bool _deleteVisible, QWidget *parent):
    QWidget(parent), vcolor(nullptr), row(-1), textVisible(true)
{
    ui.setupUi(this);

    ui.lText->setScaledContents(true);
    ui.lText->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    if(!ui.lText->layout()){
        ui.lText->setLayout(new QHBoxLayout());
    }
//    ui.lText->layout()->setMargin(2);
    ui.lText->layout()->setAlignment(ui.lText->layout(), Qt::AlignHCenter);

    connect(ui.bColor, SIGNAL(clicked(bool)), this, SLOT(onColorButtomClick(bool)));
    connect(ui.bDelete, SIGNAL(clicked(bool)), this, SLOT(onDeleteButtomClick(bool)));
    connect(ui.cbSelect, SIGNAL(toggled(bool)), this, SLOT(onSelectedChange(bool)));

    setColor(_color);
    setColorVisible(_colorVisible);
    setDeleteVisible(_deleteVisible);
    setSelectVisible(_selectVisible);
    setText(_textTitle);
//    adjustSize();
}

void FirstCollumn2::setColor(QColor _color){
    if(_color.isValid()){
        if(vcolor){
            vcolor->setData(_color);
        }
        color = _color;

        QPalette pal = ui.bColor->palette();
        pal.setColor(QPalette::Button, color);
        ui.bColor->setStyle(QStyleFactory::create("Fusion"));
        ui.bColor->setPalette(pal);
        ui.bColor->show();
    }
}

QColor FirstCollumn2::getColor() {
    if(vcolor){
        return vcolor->getData();
    }
    return color;
}

void FirstCollumn2::setText(QString _textTitle){
    textValue = _textTitle;
    ui.lText->setText(textValue);
    ui.lText->setMinimumSize(fontMetrics().width(textValue.toUpper()), fontMetrics().height());
//    ui.lText->resize(fontMetrics().width(textValue), fontMetrics().height());
    //        widgetsShow();
}

void FirstCollumn2::setSelectVisible(bool value)
{
    selectVisible = value;
    ui.cbSelect->setVisible(selectVisible);
}
