#ifndef SPRFORMULAELEMENT_H
#define SPRFORMULAELEMENT_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMenu>

#include "_types.h"
#include "variables/sprenumvariable.h"
#include "models/sprelementsmodel.h"
#include "isprwidget.h"


class SPRFormulaElement : public QLabel, public ISPRWidget
{
    Q_OBJECT

//    quint16 elementsNumber;
    SPRElementsModel *elements;
    SPREnumVariable<EnumElements> *element;
//    QMenu *contextMenu;

//    QMenu *createContextMenu();
public:
    SPRFormulaElement(QWidget *parent = nullptr);
    virtual ~SPRFormulaElement();
    void setElement(SPREnumVariable<EnumElements> *variable);

public slots:
    void setElements(SPRElementsModel *_elements);
    void setCurrentElement(EnumElements element);
    void setCurrentElement(bool trigger);
signals:
    void changeCurrentElement(EnumElements element);
protected:
//    void mousePressEvent(QMouseEvent* event);

    // ISPRWidget interface
public:
    virtual void widgetsShow();
    virtual ISPRModelData *setModelData(ISPRModelData *data);
    virtual ISPRModelData *getModelData();

protected:
    virtual void onModelChanget(IModelVariable *variable);
};

#endif // SPRFORMULAELEMENT_H
