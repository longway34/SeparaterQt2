#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "aboutdialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    AboutDialog *about;
public:
    explicit MainWindow(QString fName, QWidget *parent = nullptr);

public slots:
    void onAbout(bool);
protected slots:
    void onChangeMasterMode();
    void onCloseCommand(bool);
private:
    Ui::MainWindow ui;

    bool isMasterMode;
};

#endif // MAINWINDOW_H
