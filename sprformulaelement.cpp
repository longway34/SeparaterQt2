#include "sprformulaelement.h"


//QMenu *SPRFormulaElement::createContextMenu()
//{

//}

void SPRFormulaElement::setElement(SPRElementVariable *value)
{
    element = value;
}

SPRFormulaElement::SPRFormulaElement(QWidget *parent):
    QLabel(parent),
    element(nullptr),
    elements(nullptr)
//    contextMenu(nullptr)
{
//    setElements(&MapElementsProperty);
//    setCurrentElement(element);
    setToolTip(tr("Для смены элемента нажми правую кнопку мыши..."));
    elements = new DefaultMapElements();
}

SPRFormulaElement::~SPRFormulaElement()
{
    delete elements;
}

void SPRFormulaElement::setElements(const DefaultMapElements *elements)
{
//    this->elements = elements; // this->elementsNumber = elementsNumber;
//    if(contextMenu) delete contextMenu;
//    contextMenu = new QMenu(tr("Выбери элемент"));
    QListIterator<EnumElements> it(elements->keys());
    while (it.hasNext()) {
        EnumElements key = it.next();
        if(!this->elements->contains(key)){
            this->elements->insert(key, (*elements)[key]);
            DefaultElementsProperty prop = (*elements)[key];
            QAction *act = new QAction(prop.name+"("+prop.sname+")", this);
            act->setData(key);

            this->addAction(act);
            connect(act, SIGNAL(triggered(bool)), this, SLOT(setCurrentElement(bool)));
        }
    }
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void SPRFormulaElement::setCurrentElement(bool)
{
    EnumElements el = (EnumElements)((QAction*)sender())->data().toInt();
    if(element){
        element->setData(el);
    }
    setCurrentElement(el);
}

void SPRFormulaElement::setCurrentElement(EnumElements element)
{
    if(elements){
        if(elements->contains(element)){
            this->setText((*elements)[element].sname);
            element = element;
        } else {
            this->setText("??");
        }
    } else {
        this->setText("??");
    }
}
