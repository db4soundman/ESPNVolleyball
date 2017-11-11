#include "EspnVolleyball.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QDesktopWidget>
#include <QAction>
#include <QTextStream>
#include <QVBoxLayout>
#include <QMessageBox>

EspnVolleyball::EspnVolleyball(int& argc, char* argv[]) :
    QApplication (argc, argv) {
    setApplicationName("ESPN Volleyball Parser");
}


QString
EspnVolleyball::getAppDirPath() {
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + applicationName().replace(" ", "");
}

void
EspnVolleyball::checkAppDirectory() {
    QDir appDir(getAppDirPath());
    if (!appDir.exists()) {
        appDir.mkdir(getAppDirPath());
    }
    window.checkDefaultFile();
}

int
EspnVolleyball::exec() {
    checkAppDirectory();

    window.show();
    return QApplication::exec();
}

