#include "sprspectrumtableadv.h"



SPRSpectrumTableAdv::SPRSpectrumTableAdv(QWidget *parent): QTableWidget(parent)
{
}


void SPRSpectrumTableAdv::mousePressEvent(QMouseEvent *event)
{
    QTableWidgetItem *targetItem=itemAt(event->pos());
    if (event->button() == Qt::LeftButton) {
        // включаем драг и дроп если ячейка выделена
//        if (targetItem!=0) {
//            if ( targetItem->isSelected()) {
//                setDragDropMode(QAbstractItemView::DragDrop);
//            } else {
//                setDragDropMode(QAbstractItemView::NoDragDrop);
//            }
//        }
    }
    QTableWidget::mousePressEvent(event);
}

void SPRSpectrumTableAdv::dropEvent(QDropEvent *event)
{
    // проверяем что источник - родная таблица
    if (event->source()!=this) {
        // неродной источник обработаем обработчиком по умолчанию
        QTableWidget::dropEvent(event);
    }

    // определяем цель вставки
    int targetCol;
    QTableWidgetItem *targetItem=itemAt(event->pos());
    if (targetItem!=0) {
        targetCol=targetItem->column();
    } else {
        targetCol=columnCount();
    }
    // запоминаем номера колонок
    QTableWidgetItem *item;
    QList<int> selColumns;
    foreach (item, selectedItems()) {
        if (!selColumns.contains(item->column())) {
            // запоминаем номер строки
            selColumns.append(item->column());
        }
    }
    if (selColumns.isEmpty()) {
        event->accept();  // не будем игнорить сообщение, скажем что обработали и передадим дальше
        return;
    }
    /// сортировка массива строк по порядку т.к. в selectedItems объекты по порядку выбора, а не по возрастанию
    qSort(selColumns.begin(), selColumns.end());

    /// отключаем сортировку
    bool se=isSortingEnabled();
    setSortingEnabled(false);

    /// определяем действие
    /// без клавиш модификаторов просто сдвиг, Shift - перемещение, Ctrl - копирование, Shift+Ctrl - копирование со вставкой
    Qt::KeyboardModifiers kmod=event->keyboardModifiers();
    if (kmod & Qt::ShiftModifier) {
        /// перемещение строк
        // цель может находиться между выделениями, следовательно проверяем направление для каждой исходной строки
        int tu=0, tn=0; //счётчики обработанных и удалённых строк, для вычисления последующего смещения
        int cc=columnCount();

        foreach (int c, selColumns) {
//            insertColumn();
        }
//        foreach (int c, selColumns) {
//            if (targetCol<c) {
//                // цель выше левее
//                insertColumn(targetCol);
//                for (int i=0; i<cc; i++) {
//                    QTableWidgetItem *newitem=new QTableWidgetItem(*this->item(c+1+tu,i)); // r+1 т.к. добавили сверху строку
//                    setItem(targetCol,i,newitem);
//                    /// TODO itemWidget
//                }
//                    /// перемещение строк
//                    removeColumn(c+1); // удаляем скопированную строку
//                targetCol++;
//            }
//            if (targetCol>c) {
//                // цель правее выделения (если цель между выделениями, условие выполняется первым)
//                // т.к. список сортированный то здесь можно не учитывать обработку строк выше выделения -tn+tu
//                insertColumn(targetCol);
//                for (int i=0; i<cc; i++) {
//                    QTableWidgetItem *newitem=new QTableWidgetItem(*this->item(r-tn,i));
//                    setItem(targetCol,i,newitem);
//                    /// TODO itemWidget
//                }
//                /// перемещение строк
//                removeRow(r-tn); // удаляем скопированную строку
//                tn++;
//            }
//        }
    }
//    if ((kmod & Qt::ControlModifier) && !(kmod & Qt::ShiftModifier) || (kmod == Qt::NoModifier)) {
//        /// копирование или сдвиг значений
//        // копируем ячейки
//        QList<QTableWidgetItem *> sitems;
//        int cc=columnCount();
//        foreach (int r, selRows) {
//            for (int i=0; i<cc; i++) {
//                /// копирование
//                QTableWidgetItem *newitem=new QTableWidgetItem(*this->item(r,i));
//                sitems.append(newitem);
//                /// TODO itemWidget
//                if (kmod == Qt::NoModifier) {
//                    /// сдвиг значений
//                    // очищаем исходную ячейку
//                    setItem(r,i,new QTableWidgetItem(""));
//                }
//            }
//        }
//        // вставляем ячейки
//        QTableWidgetItem *item;
//        int c=0;
//        foreach (item, sitems) {
//            if (c>=cc) {
//                c=0;
//                targetRow++;
//            }
//            if (targetRow>=rowCount()) {
//                insertRow(targetRow);
//            }
//            setItem(targetRow,c,item);
//            /// TODO itemWidget
//            c++;
//        }
//    }

//    /// включаем сортировку
//    setSortingEnabled(se);

//    ///event->accept(); //widget дополнительно выполняет свои действия, нам этого не надо
    event->ignore(); // не передаём событие дальше    QTableWidget::dropEvent(event);
}
