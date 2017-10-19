#include "StatCrewReader.h"

#include <QDomDocument>
#include <QFile>
#include <QXmlStreamWriter>
#include <QTextStream>
#include "EspnVolleyball.h"
StatCrewReader::StatCrewReader(QObject *parent) : QObject(parent)
{
    filepath="";
}

void StatCrewReader::parseFile()
{
    QDomDocument doc;
    QFile file(filepath);
    try {
        if (!file.open(QIODevice::ReadWrite) || !doc.setContent(&file)) {
            return;
        } else {
            awayTeam.clear();
            homeTeam.clear();
        }
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
    } catch (...) {

    }
}

void StatCrewReader::writeFile()
{
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
//    QFile localFile(EspnVolleyball::getAppDirPath() + "/out.xml");
//    QTextStream stream(&localFile);
//    if (localFile.open(QIODevice::ReadWrite)) {
//        stream << output;
//    }
    localFile.close();
}

QString StatCrewReader::getFilepath() const
{
    return filepath;
}

void StatCrewReader::setFilepath(const QString &value)
{
    filepath = value;
}
