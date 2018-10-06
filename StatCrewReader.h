#ifndef STATCREWREADER_H
#define STATCREWREADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "VolleyballPlayer.h"

class StatCrewReader : public QObject
{
    Q_OBJECT
public:
    StatCrewReader(QObject *parent = 0);

    void parseFile();
    void writeFile();
    void getStats();

    bool checkDefaultFile();

    QString getFilepath() const;
    void setFilepath(const QString &value);

signals:

public slots:
    void fileIsReady(QNetworkReply* reply);
private:
    QNetworkAccessManager* manager;
    QString filepath;
    QList<VolleyballPlayer> awayTeam,homeTeam;

    void nameCorrect(VolleyballPlayer& player, QString name);
    bool gameStarted;
    bool inGame1;
};

#endif // STATCREWREADER_H
