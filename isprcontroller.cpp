#include "isprcontroller.h"

ISPRController::ISPRController(QObject *parent) : QObject(parent)
{

}

QWidget *ISPRController::addWidgetModel(WidgetsEnum widgetId, QWidget *widget, ModelsEnum modelId, ISPRModelData *model)
{
    if(widget && model){
        this->models.insert(modelId, model);
        this->widgets.insert(widgetId, widget);
        connect(model, SIGNAL(goStore()), model, SLOT(store()));
//        connect(widget, SIGNAL(), SLOT(widgetsShow()));
        return widget;
    }
}

