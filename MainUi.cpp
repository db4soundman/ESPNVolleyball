#include "MainUi.h"
#include <QFormLayout>
MainUi::MainUi(QWidget *parent) : QWidget(parent)
{
    QFormLayout* body = new QFormLayout();
    body->addRow("Away Tricode", &awayName);
    body->addRow("Home Tricode", &homeName);
    setLayout(body);
}
