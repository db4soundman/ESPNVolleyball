#include "StatCrewReader.h"

#include <QDomDocument>
#include <QFile>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QUrl>
#include <QStringList>
#include <QNetworkRequest>
#include <QTimer>
#include "EspnVolleyball.h"

#define NORMAL_GAMEPLAY 0
#define SET_OVER 1
#define WAITING_FOR_NEXT_SET 2

StatCrewReader::StatCrewReader(QObject *parent) : QObject(parent)
{
    filepath="";
    gameStarted = false;
    inGame1 = false;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileIsReady(QNetworkReply*)) );
    gameNum = 0;
    hScore = 0;
    aScore = 0;
    hPoints = 0;
    aPoints = 0;
    gameOver = false;
//    waitingForSetChange = false;
    prevGame = -1;
    prevHPoints = -1;
    prevAPoints = -1;
//    showWinText = false;
    for(int i = 0; i < 5; i++) {
        awayScores.append(0);
        homeScores.append(0);
    }
    statState = NORMAL_GAMEPLAY;
    hasWrittenRosters = false;
}

void StatCrewReader::parseFile()
{
    QDomDocument doc;
    //QFile file(EspnVolleyball::getAppDirPath() + "/in.xml");
    QFile file(filepath);
    try {
        if (!file.open(QIODevice::ReadWrite) || !doc.setContent(&file)) {
            return;
        } else {
            awayTeam.clear();
            homeTeam.clear();
        }
        try {
            if (!gameStarted) {
                QDomNode statusNode = doc.elementsByTagName("status").item(0);
                QDomNamedNodeMap atts = statusNode.attributes();
                for (int x = 0; x < atts.length(); x++) {
                    auto mapItem = atts.item(x);
                    auto attribute = mapItem.toAttr();
                    if (attribute.name() == "game") {
                        if (attribute.value() > "1") {
                            gameStarted = true;
                        } else if (attribute.value() == "1") {
                            inGame1 = true;
                        }
                    } if (inGame1 && ((attribute.name() == "vpoints" && attribute.value() > "1") ||
                                      (attribute.name() == "hpoints" && attribute.value() > "1"))) {
                        gameStarted = true;
                    }
                    if (attribute.name() == "complete" && attribute.value().toLower() == "y") {
                        gameStarted = false;
                        inGame1 = false;
                        break;
                    }
                }
            }
            if (gameStarted) {
                bool visitor = true;
                QDomNodeList teams = doc.elementsByTagName("team");
                for (int i = 0; i < teams.size(); i++) {
                    QDomNode team = teams.item(i);
                    QDomNamedNodeMap atts = team.attributes();
                    for (int x = 0; x < atts.length(); x++) {
                        auto mapItem = atts.item(x);
                        auto attribute = mapItem.toAttr();
                        if (attribute.name() == "vh") {
                            if (attribute.value() == "V" || attribute.value() == "v") {
                                visitor = true;
                            } else {
                                visitor = false;
                            }
                            break;
                        }
                    }

                    QDomElement linescore = team.firstChildElement("linescore");
                    QDomNamedNodeMap lineAtts = linescore.attributes();
                    for (int x = 0; x < lineAtts.length(); x++) {
                        auto mapItem = lineAtts.item(x);
                        auto attribute = mapItem.toAttr();
                        if (attribute.name() == "line") {
                            QStringList scores = attribute.value().split(",");
                            for (int x = 0; x < scores.length(); x++) {
                                if (visitor) {
                                    awayScores[x] = scores.at(x).toInt();
                                } else {
                                    homeScores[x] = scores.at(x).toInt();
                                }
                            }
                        }
                    }
                    QDomElement playerData = team.firstChildElement("player");
                    while (!playerData.isNull()) {
                        try {
                            VolleyballPlayer player;
                            if (playerData.attribute("name") == "TEAM") {
                                playerData = playerData.nextSiblingElement("player");
                                continue;
                            }
                            player.setFullName(playerData.attribute("name"));
                            player.setUniformStr(playerData.attribute("uni"));
                            if (playerData.attribute("gp") != "0") {
                                player.setAttackPct(playerData.firstChildElement("attack").attribute("pct"));
                                player.setKills(playerData.firstChildElement("attack").attribute("k").toInt());
                                player.setAtkErrors(playerData.firstChildElement("attack").attribute("e").toInt());

                                player.setSets(playerData.firstChildElement("set").attribute("a").toInt());

                                player.setServeErrors(playerData.firstChildElement("serve").attribute("se").toInt());
                                player.setAces(playerData.firstChildElement("serve").attribute("sa").toInt());


                                player.setDigs(playerData.firstChildElement("defense").attribute("dig").toInt());
                                player.setDigErrors(playerData.firstChildElement("defense").attribute("re").toInt());

                                player.setTotalBlocks(playerData.firstChildElement("block").attribute("tb").toInt());
                                player.setBlockErrors(playerData.firstChildElement("block").attribute("be").toInt());

                                QDomNamedNodeMap atts = team.attributes();
                                for (int x = 0; x < atts.length(); x++) {
                                    auto mapItem = atts.item(x);
                                    auto attribute = mapItem.toAttr();
                                    if (attribute.name() == "vh") {
                                        if (attribute.value() == "V" || attribute.value() == "v") {
                                            awayTeam.append(player);
                                        } else {
                                            homeTeam.append(player);
                                        }
                                        playerData = playerData.nextSiblingElement("player");
                                        break;
                                    }
                                }
                            }
                            else {
                                playerData = playerData.nextSiblingElement("player");
                            }
                        } catch (...) {
                            playerData = playerData.nextSiblingElement("player");
                        }
                    }
                }
                QDomNode statusNode = doc.elementsByTagName("status").item(0);
                QDomNamedNodeMap atts = statusNode.attributes();
                for (int x = 0; x < atts.length(); x++) {
                    auto mapItem = atts.item(x);
                    auto attribute = mapItem.toAttr();
                    if (attribute.name() == "game") {
                        gameNum = attribute.value().toInt();
                    } else if (attribute.name() == "hpoints") {
                        hPoints = attribute.value().toInt();
                    } else if (attribute.name() == "vpoints") {
                        aPoints = attribute.value().toInt();
                    } if (attribute.name() == "complete" && attribute.value().toLower() == "y") {
                        gameOver = true;
                    }
                }
                if (statState == WAITING_FOR_NEXT_SET) {
                    if (gameNum != prevGame || hPoints != prevHPoints || aPoints != prevAPoints) {
                        statState = NORMAL_GAMEPLAY;
                    }
                }
            }
        } catch (...) {
            file.close();
        }
    } catch (...) {

    }
}

void StatCrewReader::writeFile()
{
    if (gameStarted) {
        int gameNum = statState == WAITING_FOR_NEXT_SET ? this->gameNum+1:this->gameNum;
        aScore = 0;
        hScore = 0;
        for (int i = 0; i < (gameOver ? gameNum: gameNum-1); i++) {
            if (awayScores[i] > homeScores[i]) {
                aScore++;
            } else {
                hScore++;
            }
        }
//        if (waitingForSetChange && !showWinText) {
//            gameNum++;
//        }
//        if (waitingForSetChange && showWinText) {
//            return;
//        }
        QFile localFile(EspnVolleyball::getAppDirPath() + "/out.xml");
        if (!localFile.open(QIODevice::ReadWrite|QIODevice::Truncate)) {
            return;
        }
        QXmlStreamWriter writer(&localFile);
        writer.writeStartDocument();
        writer.writeStartElement("gamestats");
        writer.writeStartElement("vteam");
        for (int i = 0; i < awayTeam.size(); i++) {
            VolleyballPlayer p(awayTeam.at(i));
            p.toXml(&writer);
        }
        writer.writeEndElement();
        writer.writeStartElement("hteam");
        for (int i = 0; i < homeTeam.size(); i++) {
            VolleyballPlayer p(homeTeam.at(i));
            p.toXml(&writer);
        }
        writer.writeEndElement();
        for (int i = 1; i <= gameNum; i++) {
            if (gameNum != i) {
                if (awayScores[i-1] > homeScores[i-1]) {
                    writer.writeTextElement("game"+QString::number(i), "1");
                } else {
                    writer.writeTextElement("game"+QString::number(i), "2");
                }
            } else {
                if((hPoints > 24 || aPoints > 24) && abs(hPoints - aPoints) > 1) {
                    writer.writeTextElement("game"+QString::number(i), hPoints < aPoints ? "1" : "2");
                } else if((hPoints > 14 || aPoints > 14) && abs(hPoints - aPoints) > 1 &&i==5) {
                    writer.writeTextElement("game"+QString::number(i), hPoints < aPoints ? "1" : "2");
                }
            }
        }
        writer.writeEndElement();
        writer.writeEndDocument();
        localFile.close();
        if (!hasWrittenRosters) {
            QFile awayRoster(EspnVolleyball::getAppDirPath() + "/aroster.xml");
            if (!awayRoster.open(QIODevice::ReadWrite|QIODevice::Truncate)) {
                return;
            }
            QXmlStreamWriter awriter(&awayRoster);
            awriter.writeStartDocument();
            awriter.writeStartElement("team");
            awriter.writeStartElement("teaminfo");
            awriter.writeEndElement();
            awriter.writeStartElement("roster");
            awriter.writeStartElement("players");
            for (int i = 0; i < awayTeam.size(); i++) {
                VolleyballPlayer p(awayTeam.at(i));
                p.toRosterXml(&awriter);
            }
            awriter.writeEndElement();//players
            awriter.writeEndElement();//roster
            awriter.writeEndElement();//team
            awriter.writeEndDocument();
            awayRoster.close();

            QFile homeRoster(EspnVolleyball::getAppDirPath() + "/hroster.xml");
            if (!homeRoster.open(QIODevice::ReadWrite|QIODevice::Truncate)) {
                return;
            }
            QXmlStreamWriter hwriter(&homeRoster);
            hwriter.writeStartDocument();
            hwriter.writeStartElement("team");
            hwriter.writeStartElement("teaminfo");
            hwriter.writeEndElement();
            hwriter.writeStartElement("roster");
            hwriter.writeStartElement("players");
            for (int i = 0; i < homeTeam.size(); i++) {
                VolleyballPlayer p(homeTeam.at(i));
                p.toRosterXml(&hwriter);
            }
            hwriter.writeEndElement();//players
            hwriter.writeEndElement();//roster
            hwriter.writeEndElement();//team
            hwriter.writeEndDocument();
            homeRoster.close();
            hasWrittenRosters = true;
        }


    } else {
        QFile localFile(EspnVolleyball::getAppDirPath() + "/out.xml");
        //    QTextStream stream(&localFile);
        if (!localFile.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
            return;
        }
        //    localFile.close();
        QXmlStreamWriter writer(&localFile);
        writer.writeStartDocument();
        writer.writeStartElement("gamestats");

        writer.writeTextElement("status", "Set 1 (Best of 5)");
        writer.writeTextElement("game1", "0");
        writer.writeTextElement("game2", "0");
        writer.writeTextElement("game3", "0");
        writer.writeTextElement("game4", "0");
        writer.writeTextElement("game5", "0");
        writer.writeEndElement();
        writer.writeEndDocument();
        localFile.close();
    }
}

void StatCrewReader::getStats()
{
    manager->get(QNetworkRequest(QUrl(filepath)));
}

void StatCrewReader::fileIsReady( QNetworkReply * reply) {
    QFile inFile(EspnVolleyball::getAppDirPath() + "/in.xml");
    inFile.open(QIODevice::ReadWrite|QIODevice::Truncate);
    inFile.write(reply->readAll());
}

void StatCrewReader::setAwayName(QString name)
{
    awayName = name;
}

void StatCrewReader::setHomeName(QString name)
{
    homeName = name;
}

void StatCrewReader::setWinningChar(QString value)
{
    setWinnerChar = value;
}

void StatCrewReader::enterWaitingState()
{

}

void StatCrewReader::advanceSetInWaitingState()
{
    if (statState == SET_OVER)
        statState = WAITING_FOR_NEXT_SET;
}

void StatCrewReader::startWait()
{
    prevGame = gameNum;
    prevAPoints = aPoints;
    prevHPoints = hPoints;
    statState = SET_OVER;
    QTimer::singleShot(1000 * 45, this, SLOT(advanceSetInWaitingState()));
}

bool StatCrewReader::checkDefaultFile()
{
    QString temp = "";
    QFile defaultFile(EspnVolleyball::getAppDirPath()+"/filepath.txt");
    if (defaultFile.exists() && defaultFile.open(QIODevice::ReadWrite)) {
        QTextStream stream(&defaultFile);
        temp = stream.readAll().trimmed();
        QFile firstFile(temp);
        if (firstFile.exists()) {
            filepath = temp;
            return true;
        }
    }
    return false;
}

QString StatCrewReader::getFilepath() const
{
    return filepath;
}

void StatCrewReader::setFilepath(const QString &value)
{
    filepath = value;
    hasWrittenRosters = false;
}
