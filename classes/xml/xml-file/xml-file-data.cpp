#include "xml-file.h"

QString XmlFile::openData()
{
    auto xmlFile = new QFile(this->path());
    if (!xmlFile->open(QIODevice::ReadOnly)) {
        qDebug() << QString("Impossibile aprire '%1'").arg(this->path());
        return "";
    }
    auto xmlData = xmlFile->readAll();
    xmlFile->close();
    xmlFile->deleteLater();
    return xmlData;
}
