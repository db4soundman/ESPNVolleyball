#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "StatCrewReader.h"
#include <QTimer>
#include "MainUi.h"
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void checkDefaultFile();

signals:
    void updatePath(QString);
    
private slots:
    void browseForFile();
    void handleTimeout();

private:
    MainUi ui;
    StatCrewReader reader;
    QTimer timer;

};

#endif // MAINWINDOW_H
