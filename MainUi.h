#ifndef MAINUI_H
#define MAINUI_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>

class MainUi : public QWidget
{
    Q_OBJECT
public:
    explicit MainUi(QWidget *parent = nullptr);

    QLineEdit homeName, awayName, setWinnerMarking;

signals:

public slots:

};

#endif // MAINUI_H
