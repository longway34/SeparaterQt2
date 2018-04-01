#ifndef SPRFORMULAELEMENT_H
#define SPRFORMULAELEMENT_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMenu>

#include "_types.h"
#include "models/sprelementvariable.h"


class SPRFormulaElement : public QLabel
{
    Q_OBJECT

//    quint16 elementsNumber;
    DefaultMapElements *elements;
    SPRElementVariable *element;
//    QMenu *contextMenu;

//    QMenu *createContextMenu();
public:
    SPRFormulaElement(QWidget *parent = nullptr);
    virtual ~SPRFormulaElement();
    void setElement(SPRElementVariable *value);

public slots:
    void setElements(const DefaultMapElements *elements);
    void setCurrentElement(EnumElements element);
    void setCurrentElement(bool trigger);
signals:
    void changeCurrentElement(EnumElements element);
protected:
//    void mousePressEvent(QMouseEvent* event);
};

#endif // SPRFORMULAELEMENT_H
