#include "sprformulaelement.h"


//QMenu *SPRFormulaElement::createContextMenu()
//{

//}

void SPRFormulaElement::setElement(SPREnumVariable<EnumElements> *variable)
{
    element = variable;
}

SPRFormulaElement::SPRFormulaElement(QWidget *parent):
    QLabel(parent), ISPRWidget(),
    element(nullptr),
    elements(nullptr)
//    contextMenu(nullptr)
{
//    setElements(&MapElementsProperty);
//    setCurrentElement(element);
    setToolTip(tr("Для смены элемента нажми правую кнопку мыши..."));
//    elements = new DefaultMapElements();
}

SPRFormulaElement::~SPRFormulaElement()
{
    if(elements && property("delete_elements").value<bool>()){
        delete elements;
    }
}

void SPRFormulaElement::setElements(SPRElementsModel *_elements)
{
//    this->elements = elements; // this->elementsNumber = elementsNumber;
//    if(contextMenu) delete contextMenu;
//    contextMenu = new QMenu(tr("Выбери элемент"));
}

void SPRFormulaElement::setCurrentElement(bool)
{
    EnumElements el = (EnumElements)((QAction*)sender())->data().toInt();
    if(element){
        element->setData(el);
    }
    setCurrentElement(el);
}

void SPRFormulaElement::setCurrentElement(EnumElements _element)
{
    if(elements){
        if(elements->getElements().contains(_element)){
            this->setText(elements->getElementProperty(_element, typeUsedElements)->sName->getData());
        } else {
            this->setText("??");
        }
    } else {
        this->setText("??");
    }
}


void SPRFormulaElement::widgetsShow()
{
    if(elements){
        blockSignals(true);
        while(actions().size() > 0){
            QAction *act = actions()[0];
            disconnect(act, SIGNAL(triggered(bool)), this, SLOT(setCurrentElement(bool)));
            removeAction(act);
            delete act;
        }
        foreach(EnumElements el, elements->getElements().keys()){
            SPRElementsProperty *prop = elements->getElementProperty(el, typeUsedElements);

            QAction *act = new QAction(QString(prop->fName->getData().toUtf8()+"("+prop->sName->getData().toUtf8()+")"), this);
            act->setData(el);

            this->addAction(act);
            connect(act, SIGNAL(triggered(bool)), this, SLOT(setCurrentElement(bool)));

            if(element){
                EnumElements el = element->getData();
                if(!elements->getElements().keys().contains(el)){
                    element->setData(Ns);
                }
                setCurrentElement(element->getData());

            }
        }
//        QListIterator<EnumElements> it(_elements->keys());
//        while (it.hasNext()) {
//            EnumElements key = it.next();
//            if(!this->elements->contains(key)){
//                this->elements->insert(key, (*_elements)[key]);
//                DefaultElementsProperty prop = (*_elements)[key];
//                QAction *act = new QAction(prop.name+"("+prop.sname+")", this);
//                act->setData(key);

//                this->addAction(act);
//                connect(act, SIGNAL(triggered(bool)), this, SLOT(setCurrentElement(bool)));
//            }
//        }
        this->setContextMenuPolicy(Qt::ActionsContextMenu);
        blockSignals(false);
    }
}

ISPRModelData *SPRFormulaElement::setModelData(ISPRModelData *data)
{
    if(data){
        if(elements != data){
            elements = (SPRElementsModel*)data;
    //        setProperty("delete_elements", QVariant(false));
            widgetsShow();
        }
    }
}

ISPRModelData *SPRFormulaElement::getModelData()
{
    return elements;
}

void SPRFormulaElement::onModelChanget(IModelVariable *variable)
{
    if(variable == element){
        widgetsShow();
    }
}
