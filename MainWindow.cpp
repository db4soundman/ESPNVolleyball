#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFormLayout>

void MainWindow::checkDefaultFile()
{
    if (!reader.checkDefaultFile()) {
        QMessageBox msg;
        msg.setText("No file currently set. Please set a file to begin parsing.");
        msg.exec();
    } else{
        timer.start();

    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    connect(&ui, SIGNAL(needToBrowse()), this, SLOT(browseForFile()));
    timer.setInterval(1500);
    connect(&timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    connect(this, SIGNAL(updatePath(QString)), &ui, SLOT(updatePath(QString)));
    setCentralWidget(&mainView);
    infoDock.setWidget(&ui);

    addDockWidget(Qt::TopDockWidgetArea, &infoDock);

    connect(&ui.awayName, SIGNAL(textEdited(QString)), &reader, SLOT(setAwayName(QString)));
    connect(&ui.homeName, SIGNAL(textEdited(QString)), &reader, SLOT(setHomeName(QString)));
    connect(&ui.awayTri, SIGNAL(textEdited(QString)), &reader, SLOT(setAwayTri(QString)));
    connect(&ui.homeTri, SIGNAL(textEdited(QString)), &reader, SLOT(setHomeTri(QString)));
    connect(&ui.awayName, SIGNAL(textEdited(QString)), &mainView, SLOT(setAwayName(QString)));
    connect(&ui.homeName, SIGNAL(textEdited(QString)), &mainView, SLOT(setHomeName(QString)));

    connect(&reader, SIGNAL(awayStatsUpdated(QList<VolleyballPlayer>)), &mainView, SLOT(updateAwayStats(QList<VolleyballPlayer>)));
    connect(&reader, SIGNAL(homeStatsUpdated(QList<VolleyballPlayer>)), &mainView, SLOT(updateHomeStats(QList<VolleyballPlayer>)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::browseForFile()
{
    timer.stop();
    QString path="";
    path = QFileDialog::getOpenFileName(0, "Stat Crew File");
    if (!path.isEmpty()) {
        reader.setFilepath(path);
        timer.start();
        emit updatePath(path);
    }
}

void MainWindow::handleTimeout()
{
//    reader.getStats();
    reader.parseFile();
    reader.writeFile();
}
