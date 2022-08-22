#include "archivio.h"

#include <QDir>
#include <QSettings>
#include <QStandardPaths>

void Archivio::init()
{
    this->initSettings();
    this->initWatcher();
    this->initList();
}

void Archivio::initSettings()
{
    static QSettings settings("settings.ini", QSettings::IniFormat);

    auto folder = settings.value("folder");
    if(!folder.isValid())
        folder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    this->setXmlFolder(folder.toString());
    connect(this, &Archivio::xmlFolderChanged, this, [=]{ settings.setValue("folder", this->xmlFolder()); });

    auto spese = settings.value("spese");
    if(spese.isValid())
        this->setSpese(spese.toStringList());

}

void Archivio::initWatcher()
{
    auto updateWatcher = [=]{
        if(!this->watcher()->files().isEmpty())
            this->watcher()->removePaths(this->watcher()->files());

        if(this->xmlFolder().isEmpty())
            return;

        this->watcher()->addPath(this->xmlFolder());
    };
    connect(this, &Archivio::xmlFolderChanged, this, updateWatcher);
    updateWatcher();
}


void Archivio::initList()
{
    auto updateList = [=]{

        auto children = this->findChildren<XmlFile*>();
        for(auto child : qAsConst(children))
            child->deleteLater();
        this->xmlList()->clear();

        QDir dir(this->xmlFolder());
        QStringList files = dir.entryList({"*.xml", "*.XML", "*.xml.p7m", "*.XML.P7M"},QDir::Files);

        for(const auto &file : qAsConst(files)){
            auto path = QString("%1/%2").arg(dir.absolutePath(),file);
            QFileInfo info(path);
            auto extension = info.suffix().toLower();
            XmlFile *xml = extension=="p7m" ? (new XmlP7mFile(this)) : (new XmlFile(this));
            xml->setPath(path);
            this->xmlList()->append(xml);
        }


    };
    connect(this, &Archivio::xmlFolderChanged, this, updateList);
    connect(this->watcher(), &QFileSystemWatcher::directoryChanged, this, updateList);
    updateList();
}
