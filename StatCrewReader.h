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
    void setAwayName(QString name);
    void setHomeName(QString name);
    void setWinningChar(QString value);
private:
    QNetworkAccessManager* manager;
    QString filepath, homeName, awayName, setWinnerChar;
    QList<VolleyballPlayer> awayTeam,homeTeam;

    void nameCorrect(VolleyballPlayer& player, QString name);
    int gameNum, hScore, aScore, hPoints, aPoints;
    bool gameStarted, gameOver;
    bool inGame1;
};

#endif // STATCREWREADER_H
