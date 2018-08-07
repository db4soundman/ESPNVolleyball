#include "VolleyballPlayer.h"
#include <QXmlStreamWriter>
#include <algorithm>
#include <functional>

#define KILLS 0
#define SETS 1
#define ACES 2
#define DIGS 3
#define BLOCKS 4

//#include <QPair>

VolleyballPlayer::VolleyballPlayer(QObject *parent) : QObject(parent)
{
    // Vitals
     fullName = "";
     uniformStr = "";
     uniformNum = 0;

    // Attack stats
     totalAttack = 0;
     kills = 0;
     atkErrors = 0;
     attackPct = "";
    // Set stats
     sets = 0;
    // Serve stats
     serveErrors = 0;
     aces = 0;
    // Dig stats
     digs = 0;
     digErrors = 0;
    // Block stats
     totalBlocks = 0;
     blockErrors = 0;
     blockAttempts = 0;
}

VolleyballPlayer::VolleyballPlayer(const VolleyballPlayer &p)
{
    // Vitals
     fullName = p.fullName;
     uniformStr = p.uniformStr;
     uniformNum = p.uniformNum;

    // Attack stats
     totalAttack = p.totalAttack;
     kills = p.kills;
     atkErrors = p.atkErrors;
     attackPct = p.attackPct;
    // Set stats
     sets = p.sets;
    // Serve stats
     serveErrors = p.serveErrors;
     aces = p.aces;
    // Dig stats
     digs = p.digs;
     digErrors = p.digErrors;
    // Block stats
     totalBlocks = p.totalBlocks;
     blockErrors = p.blockErrors;
     blockAttempts = p.blockAttempts;
}

VolleyballPlayer &VolleyballPlayer::operator=(const VolleyballPlayer &p)
{
    // Vitals
     fullName = p.fullName;
     uniformStr = p.uniformStr;
     uniformNum = p.uniformNum;

    // Attack stats
     totalAttack = p.totalAttack;
     kills = p.kills;
     atkErrors = p.atkErrors;
     attackPct = p.attackPct;
    // Set stats
     sets = p.sets;
    // Serve stats
     serveErrors = p.serveErrors;
     aces = p.aces;
    // Dig stats
     digs = p.digs;
     digErrors = p.digErrors;
    // Block stats
     totalBlocks = p.totalBlocks;
     blockErrors = p.blockErrors;
     blockAttempts = p.blockAttempts;
     return *this;
}

void VolleyballPlayer::toXml(QXmlStreamWriter *stream)
{
   // QXmlStreamWriter stream(dest);
    stream->setAutoFormatting(true);
    stream->writeStartElement("player");
    stream->writeAttribute("name", fullName);
    stream->writeAttribute("uni", uniformStr);

    QList<int> stats;
    stats.append(kills);
    stats.append(sets);
    stats.append(aces);
    stats.append(digs);
    stats.append(totalBlocks);
    std::sort(stats.begin(), stats.end(), std::greater<int>());
    bool twoCats = stats[0] - stats[1] < stats[1];
    int numCats = stats[1] - stats[2] < stats[2] ? 3 : twoCats? 2 : 1;
    stream->writeTextElement("labelone", getStatLabel(stats[0]));
    stream->writeTextElement("statone", QString::number(stats[0]));

    if (numCats == 1) {
        // Show supporting stats for the category of the primary stat displayed.
        QString label = getStatLabel(stats[0]);
        if (label == "KILLS") {
            stream->writeTextElement("labeltwo", "HIT PCT");
            stream->writeTextElement("stattwo", attackPct);
            stream->writeTextElement("numstats", QString::number(2));
        } else if (label == "ASSISTS") {
            if (digs >= kills) {
                stream->writeTextElement("labeltwo", "DIGS");
                stream->writeTextElement("stattwo", QString::number(digs));
            } else {
                stream->writeTextElement("labeltwo", "KILLS");
                stream->writeTextElement("stattwo", QString::number(kills));
            }
            stream->writeTextElement("numstats", QString::number(2));
        } else if (label == "DIGS") {
            stream->writeTextElement("labeltwo", "ERRORS");
            stream->writeTextElement("stattwo", QString::number(digErrors));
            stream->writeTextElement("numstats", QString::number(2));
        } else {
            // Blocks
            stream->writeTextElement("labeltwo", "ERRORS");
            stream->writeTextElement("stattwo", QString::number(blockErrors));
            stream->writeTextElement("numstats", QString::number(2));
        }
    } else if (numCats > 1) {
        // We need to get the other stats
        if (stats[0] == stats[1]) {
            // Perform reverse lookup
            stream->writeTextElement("labeltwo", getStatLabelReverse(stats[1]));
        } else {
            // It's unique, go get the label
            stream->writeTextElement("labeltwo", getStatLabel(stats[1]));
        }
        stream->writeTextElement("stattwo", QString::number(stats[1]));

        if (numCats == 3) {
            if (stats[1] == stats[2]) {
                // Perform reverse lookup
                stream->writeTextElement("labelthree", getStatLabelReverse(stats[2]));
            } else {
                // It's unique, go get the label
                stream->writeTextElement("labelthree", getStatLabel(stats[2]));
            }
            stream->writeTextElement("statthree", QString::number(stats[2]));
            stream->writeTextElement("numstats", QString::number(3));
        } else {
            stream->writeTextElement("numstats", QString::number(2));
        }
    }

    stream->writeEndElement();// player

}

QString VolleyballPlayer::getStatLabel(int stat)
{
    if (stat == kills) return "KILLS";
    if (stat == sets) return "ASSISTS";
    if (stat == digs) return "DIGS";
    if (stat == totalBlocks) return "BLOCKS";
    return "";
}

QString VolleyballPlayer::getStatLabelReverse(int stat)
{
    if (stat == totalBlocks) return "BLOCKS";
    if (stat == digs) return "DIGS";
    if (stat == sets) return "ASSISTS";
    if (stat == kills) return "KILLS";
    return "";
}

QString VolleyballPlayer::getFullName() const
{
    return fullName;
}

void VolleyballPlayer::setFullName(const QString &value)
{
    fullName = value;
}

QString VolleyballPlayer::getUniformStr() const
{
    return uniformStr;
}

void VolleyballPlayer::setUniformStr(const QString &value)
{
    uniformStr = value;
    uniformNum = value.toInt();
}

int VolleyballPlayer::getUniformNum() const
{
    return uniformNum;
}

void VolleyballPlayer::setUniformNum(int value)
{
    uniformNum = value;
}

int VolleyballPlayer::getTotalAttack() const
{
    return totalAttack;
}

void VolleyballPlayer::setTotalAttack(int value)
{
    totalAttack = value;
}

int VolleyballPlayer::getKills() const
{
    return kills;
}

void VolleyballPlayer::setKills(int value)
{
    kills = value;
}

int VolleyballPlayer::getAtkErrors() const
{
    return atkErrors;
}

void VolleyballPlayer::setAtkErrors(int value)
{
    atkErrors = value;
}

QString VolleyballPlayer::getAttackPct() const
{
    return attackPct;
}

void VolleyballPlayer::setAttackPct(const QString &value)
{
    attackPct = value;
}

int VolleyballPlayer::getSets() const
{
    return sets;
}

void VolleyballPlayer::setSets(int value)
{
    sets = value;
}

int VolleyballPlayer::getServeErrors() const
{
    return serveErrors;
}

void VolleyballPlayer::setServeErrors(int value)
{
    serveErrors = value;
}

int VolleyballPlayer::getAces() const
{
    return aces;
}

void VolleyballPlayer::setAces(int value)
{
    aces = value;
}

int VolleyballPlayer::getDigs() const
{
    return digs;
}

void VolleyballPlayer::setDigs(int value)
{
    digs = value;
}

int VolleyballPlayer::getDigErrors() const
{
    return digErrors;
}

void VolleyballPlayer::setDigErrors(int value)
{
    digErrors = value;
}

int VolleyballPlayer::getTotalBlocks() const
{
    return totalBlocks;
}

void VolleyballPlayer::setTotalBlocks(int value)
{
    totalBlocks = value;
}

int VolleyballPlayer::getBlockErrors() const
{
    return blockErrors;
}

void VolleyballPlayer::setBlockErrors(int value)
{
    blockErrors = value;
}

int VolleyballPlayer::getBlockAttempts() const
{
    return blockAttempts;
}

void VolleyballPlayer::setBlockAttempts(int value)
{
    blockAttempts = value;
}
