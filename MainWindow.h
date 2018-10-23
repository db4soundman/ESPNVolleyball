#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "StatCrewReader.h"
#include <QTimer>
#include "MainUi.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void checkDefaultFile();
    
private slots:
    void browseForFile();
    void handleTimeout();

private:
    MainUi ui;
    QPushButton fileButton;
    StatCrewReader reader;
    QTimer timer;
};

#endif // MAINWINDOW_H
