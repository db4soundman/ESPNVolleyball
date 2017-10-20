#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    fileButton.setText("Set File");
    connect(&fileButton, SIGNAL(clicked(bool)), this, SLOT(browseForFile()));
    timer.setInterval(1000*3);
    connect(&timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    setCentralWidget(&fileButton);
    if (reader.checkDefaultFile()) {
        QMessageBox msg;
        msg.setText("Found the default file, you should be good.");
        msg.exec();
        timer.start();

    } else {
        QMessageBox msg;
        msg.setText("No file currently set. Please set a file to begin parsing.");
        msg.exec();
    }
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
    reader.parseFile();
    reader.writeFile();
}
