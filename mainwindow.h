#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

protected slots:
    void onChangeMasterMode();
private:
    Ui::MainWindow ui;

    bool isMasterMode;
};

#endif // MAINWINDOW_H
