#include "archivio.h"

Archivio::Archivio(QObject *parent) :
    QObject{parent},
    m_xmlList(new XmlList{this}),
    m_watcher(new QFileSystemWatcher{this})
{
    this->init();
}

Archivio::~Archivio()
{
    auto children = this->findChildren<XmlFile*>();
    for(auto child : qAsConst(children))
        child->deleteLater();
    this->xmlList()->clear();

    m_xmlList->deleteLater();
    m_watcher->deleteLater();
}
