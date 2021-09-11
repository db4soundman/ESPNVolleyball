#include "MainUi.h"
#include <QFormLayout>
MainUi::MainUi(QWidget *parent) : QWidget(parent)
{
    QFormLayout* body = new QFormLayout();
    body->addRow("Away Name", &awayName);
    body->addRow("Home Name", &homeName);
    body->addRow("Away Tri", &awayTri);
    body->addRow("Home Tri", &homeTri);
    fileButton.setText("Set File");
    body->addRow(&fileButton);
    body->addRow(&statFile);
    connect(&fileButton, SIGNAL(clicked(bool)), this, SIGNAL(needToBrowse()));
//    body->addRow("Set Winner Character", &setWinnerMarking);
    setLayout(body);
}

void MainUi::updatePath(QString path)
{
    statFile.setText(path);
}
