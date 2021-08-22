#ifndef VOLLEYBALLPLAYER_H
#define VOLLEYBALLPLAYER_H

#include <QObject>
#include <QXmlStreamWriter>

class VolleyballPlayer : public QObject
{
    Q_OBJECT
public:
    VolleyballPlayer(QObject *parent = 0);
    VolleyballPlayer(const VolleyballPlayer& p);
    VolleyballPlayer& operator=(const VolleyballPlayer& p);

    void toXml(QXmlStreamWriter* stream);

    QString getStatLabel(int stat);
    QString getStatLabelReverse(int stat);

    QString getFullName() const;
    void setFullName(const QString &value);

    QString getUniformStr() const;
    void setUniformStr(const QString &value);

    int getUniformNum() const;
    void setUniformNum(int value);

    int getTotalAttack() const;
    void setTotalAttack(int value);

    int getKills() const;
    void setKills(int value);

    int getAtkErrors() const;
    void setAtkErrors(int value);

    QString getAttackPct() const;
    void setAttackPct(const QString &value);

    int getSets() const;
    void setSets(int value);

    int getServeErrors() const;
    void setServeErrors(int value);

    int getAces() const;
    void setAces(int value);

    int getDigs() const;
    void setDigs(int value);

    int getDigErrors() const;
    void setDigErrors(int value);

    int getTotalBlocks() const;
    void setTotalBlocks(int value);

    int getBlockErrors() const;
    void setBlockErrors(int value);

    int getBlockAttempts() const;
    void setBlockAttempts(int value);

    void toRosterXml(QXmlStreamWriter *stream);

signals:

public slots:

private:
    // Vitals
    QString fullName,uniformStr;
    int uniformNum;

    // Attack stats
    int totalAttack, kills, atkErrors;
    QString attackPct;
    // Set stats
    int sets;
    // Serve stats
    int serveErrors, aces;
    // Dig stats
    int digs, digErrors;
    // Block stats
    int totalBlocks, blockErrors, blockAttempts;


};

#endif // VOLLEYBALLPLAYER_H
