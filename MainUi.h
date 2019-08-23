#ifndef MAINUI_H
#define MAINUI_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class MainUi : public QWidget
{
    Q_OBJECT
public:
    explicit MainUi(QWidget *parent = nullptr);

    QLineEdit homeName, awayName, setWinnerMarking;

signals:
    void needToBrowse();

public slots:
    void updatePath(QString path);

private:
    QLabel statFile;
    QPushButton fileButton;

};

#endif // MAINUI_H
