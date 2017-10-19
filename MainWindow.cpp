#include "MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    fileButton.setText("Set File");
    connect(&fileButton, SIGNAL(clicked(bool)), this, SLOT(browseForFile()));
    timer.setInterval(1000*3);
    connect(&timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    setCentralWidget(&fileButton);
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