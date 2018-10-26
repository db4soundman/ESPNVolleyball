#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>

void MainWindow::checkDefaultFile()
{
//    if (!reader.checkDefaultFile()) {
//        QMessageBox msg;
//        msg.setText("No file currently set. Please set a file to begin parsing.");
//        msg.exec();
//    } else{
    reader.checkDefaultFile();
        timer.start();
//    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    fileButton.setText("Set File");
    connect(&fileButton, SIGNAL(clicked(bool)), this, SLOT(browseForFile()));
    timer.setInterval(1500);
    connect(&timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    setCentralWidget(&ui);

    connect(&ui.awayName, SIGNAL(textEdited(QString)), &reader, SLOT(setAwayName(QString)));
    connect(&ui.homeName, SIGNAL(textEdited(QString)), &reader, SLOT(setHomeName(QString)));
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
    }
}

void MainWindow::handleTimeout()
{
    reader.getStats();
    reader.parseFile();
    reader.writeFile();
}
