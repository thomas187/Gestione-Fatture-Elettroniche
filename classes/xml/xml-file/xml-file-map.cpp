#include "xml-file.h"

QList<QPair<QString,QString>> XmlFile::getList(QString xmlData)
{
    auto xmlReader = new QXmlStreamReader(xmlData);
    xmlReader->setNamespaceProcessing(true);

    QList<QPair<QString,QString>> list;
    int kDepth = 0;
    while(!xmlReader->atEnd() && !xmlReader->hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader->readNext();

        if(token == QXmlStreamReader::StartElement){
            kDepth++;

            QString name = xmlReader->name().toString();
            QString text = "-";

            if(list.isEmpty() || kDepth<=1){
                list.append({name,text});
                continue;
            }

            auto last = list.last().first;
            text = list.last().second;
            auto i = list.count()-1;
            auto lDepth = last.split("/").count();

            if(kDepth>1 && kDepth<=lDepth){
                auto count = kDepth-1;
                list.append({last.split("/").mid(0,count).join("/")+"/"+name,text});
                continue;
            }

            auto item = last+"/"+name;
            list.replace(i,{item,text});
            continue;
        }

        if(token == QXmlStreamReader::Characters){
            auto last = list.last().first;
            QString text = xmlReader->text().toString().remove(QRegExp("^ +")).remove(QRegExp(" +$"))
                    .remove("\n").remove("\r").remove("\t").remove("\n");
            if(text.isEmpty())
                continue;
            auto i = list.count()-1;
            list.replace(i,{last,text});
            continue;
        }

        if(token == QXmlStreamReader::EndElement){
            kDepth--;
            continue;
        }
    }
    delete xmlReader;
    return list;
}

void XmlFile::createMap()
{
    auto list = this->list();

    QMap<QString,QString> map;
    for(const auto &item : qAsConst(list)){
        if(map.contains(item.first))
            continue;
        map.insert(item.first, item.second);
    }

    this->setMap(map);
}
