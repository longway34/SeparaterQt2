#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QColor>
#include <QColorDialog>

class ColorButton : public QPushButton
{
    Q_OBJECT

    QColor color;
public:
    ColorButton(QWidget *parent = nullptr);
    ColorButton(QColor color, QWidget *parent = nullptr);

    void setColor(QColor _color){
            color = _color;
            QString gradFormat = "QPushButton{background-color: qlineargradient(spread:pad, x1: 0, y1: 0, x2: 0, y2: 1,"
                    "stop: 0 rgba(%d, %d, %d, 64),"
                    "stop: 0.5 rgba(%d, %d, %d, 255),"
                    "stop: 1 rgba(%d, %d, %d, 64)); border-radius: 3px; border: 1px solid #8f8f91}";
            QString gradFormatPress = "QPushButton:pressed{background-color: qlineargradient(spread:pad, x1: 0, y1: 0, x2: 0, y2: 1,"
                    "stop: 0 rgba(%d, %d, %d, 128),"
                    "stop: 0.5 rgba(%d, %d, %d, 255),"
                    "stop: 1 rgba(%d, %d, %d, 128)); border: 2px solid #8f8f91}";
            QString gradFormatHover = "QPushButton:hover{background-color: qlineargradient(spread:pad, x1: 0, y1: 0, x2: 0, y2: 1,"
                    "stop: 0 rgba(%d, %d, %d, 64),"
                    "stop: 0.5 rgba(%d, %d, %d, 128),"
                    "stop: 1 rgba(%d, %d, %d, 255))}";

            QString ssh; ssh.sprintf(gradFormat.toStdString().c_str(), color.red(), color.green(), color.blue(),
                                                 color.red(), color.green(), color.blue(),
                                                 color.red(), color.green(), color.blue());
            QString sshp; sshp.sprintf(gradFormatPress.toStdString().c_str(), color.red(), color.green(), color.blue(),
                                                 color.red(), color.green(), color.blue(),
                                                 color.red(), color.green(), color.blue());
            QString sshph; sshph.sprintf(gradFormatHover.toStdString().c_str(), color.red(), color.green(), color.blue(),
                                                 color.red(), color.green(), color.blue(),
                                                 color.red(), color.green(), color.blue());

            ssh += sshp + sshph;
            setStyleSheet(ssh);
    }
public slots:
    void onClickMouse(bool){
        QColor clr = QColorDialog::getColor(this->color, this);
        if(clr != color && clr.isValid()){
            setColor(clr);
            emit changeColor(color);
        }
    }
signals:
    void changeColor(QColor);
};

#endif // COLORBUTTON_H
