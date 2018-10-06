#include "StatCrewReader.h"

#include <QDomDocument>
#include <QFile>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QUrl>
#include <QNetworkRequest>
#include "EspnVolleyball.h"
StatCrewReader::StatCrewReader(QObject *parent) : QObject(parent)
{
    filepath="";
    gameStarted = false;
    inGame1 = false;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileIsReady(QNetworkReply*)) );

}

void StatCrewReader::parseFile()
{
    QDomDocument doc;
    QFile file(EspnVolleyball::getAppDirPath() + "/in.xml");
    try {
        if (!file.open(QIODevice::ReadWrite) || !doc.setContent(&file)) {
            return;
        } else {
            awayTeam.clear();
            homeTeam.clear();
        }
        if (!gameStarted) {
            QDomNode statusNode = doc.elementsByTagName("status").item(0);
            QDomNamedNodeMap atts = statusNode.attributes();
            for (int x = 0; x < atts.length(); x++) {
                auto mapItem = atts.item(x);
                auto attribute = mapItem.toAttr();
                if (attribute.name() == "game") {
                    if (attribute.value() > "1") {
                        gameStarted = true;
                        break;
                    } else if (attribute.value() == "1") {
                        inGame1 = true;
                    }
                } else if (inGame1 && ((attribute.name() == "vpoints" && attribute.value() > "0") ||
                                       (attribute.name() == "hpoints" && attribute.value() > "0"))) {
                    gameStarted = true;
                } else if (attribute.name() == "complete" && attribute.value().toLower() == "y") {
                    gameStarted = false;
                    inGame1 = false;
                    break;
                }
            }
        }
        if (gameStarted) {
            QDomNodeList teams = doc.elementsByTagName("team");
            for (int i = 0; i < teams.size(); i++) {
                QDomNode team = teams.item(i);
                QDomElement playerData = team.firstChildElement("player");
                while (!playerData.isNull()) {
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
                }
            }
        }
    } catch (...) {

    }
}

void StatCrewReader::writeFile()
{
    if (gameStarted) {
        QFile localFile(EspnVolleyball::getAppDirPath() + "/out.xml");
        //    QTextStream stream(&localFile);
        if (!localFile.open(QIODevice::ReadWrite)) {
            return;
        }
        //    localFile.close();
        QString output = "";
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
  inFile.open(QIODevice::ReadWrite);
  inFile.write(reply->readAll());
}

bool StatCrewReader::checkDefaultFile()
{
    QString temp = "";
    QFile defaultFile(EspnVolleyball::getAppDirPath()+"/filepath.txt");
    if (defaultFile.exists() && defaultFile.open(QIODevice::ReadWrite)) {
        QTextStream stream(&defaultFile);
        filepath = stream.readAll().trimmed();
//        QFile firstFile(temp);
//        if (firstFile.exists()) {
//            filepath = temp;
//            return true;
//        }
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
}
