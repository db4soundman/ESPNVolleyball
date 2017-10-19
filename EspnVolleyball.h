#ifndef ESPNVOLLEYBALL_H
#define ESPNVOLLEYBALL_H

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "MainWindow.h"
#include <QImage>
#include <QPixmap>
#include <QWidget>

class EspnVolleyball : public QApplication
{
public:
    EspnVolleyball(int &argc, char *argv[]);
    static QString getAppDirPath();
    void checkAppDirectory();
    int exec();

private:
    MainWindow window;
};

#endif // ESPNVOLLEYBALL_H
