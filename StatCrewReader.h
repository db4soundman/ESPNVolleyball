#ifndef STATCREWREADER_H
#define STATCREWREADER_H

#include <QObject>
#include "VolleyballPlayer.h"

class StatCrewReader : public QObject
{
    Q_OBJECT
public:
    StatCrewReader(QObject *parent = 0);

    void parseFile();
    void writeFile();

    QString getFilepath() const;
    void setFilepath(const QString &value);

signals:

public slots:

private:
    QString filepath;
    QList<VolleyballPlayer> awayTeam,homeTeam;

    void nameCorrect(VolleyballPlayer& player, QString name);
};

#endif // STATCREWREADER_H
