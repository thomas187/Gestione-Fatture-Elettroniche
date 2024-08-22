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

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
void Archivio::initList()
{

    auto fetchInvoicePaths = [=](const QString &dbPath, const QDate &minDate, const QDate &maxDate)->QStringList {

        if (QSqlDatabase::contains("qt_sql_default_connection"))
            QSqlDatabase::removeDatabase("qt_sql_default_connection");

        // Connessione al database SQLite
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbPath);

        if (!db.open()) {
            qDebug() << "Errore nell'aprire il database:" << db.lastError().text();
            qDebug() << "Percorso database:" << dbPath;
            return {};
        }

        // Query per selezionare i percorsi delle fatture tra minDate e maxDate
        QSqlQuery query;
        query.prepare("SELECT File FROM tbl_fatture WHERE data BETWEEN ? AND ?");

        // Bind delle variabili
        query.addBindValue(minDate.toString("yyyy-MM-dd"));
        query.addBindValue(maxDate.toString("yyyy-MM-dd"));

        // Esecuzione della query
        if (!query.exec()) {
            qDebug() << "Errore nella query:" << query.lastError().text();
            db.close();
            return {};
        }

        // Stampa i percorsi delle fatture trovate
        QStringList files;
        while (query.next()) {
            QString path = query.value(0).toString();
            files += path;
        }

        // Chiudi la connessione al database
        db.close();
        return files;
    };

    auto updateList = [=]{

        QDir dir(this->xmlFolder());
        QString relativo = "..\\database\\sdipec.sqlite";
        QString assoluto = dir.absoluteFilePath(relativo);
        assoluto.replace("\\","/");

        QStringList files;
        if (QFile::exists(assoluto) && !this->showAll()) {
            QDate minDate = QDate::fromString(this->filterFromDate(), "yyyy-MM-dd");
            QDate maxDate = QDate::fromString(this->filterToDate(), "yyyy-MM-dd");
            maxDate = maxDate.addMonths(1);
            files = fetchInvoicePaths(assoluto, minDate, maxDate);
        } else {
            files = dir.entryList({"*.xml", "*.XML", "*.xml.p7m", "*.XML.P7M"},QDir::Files);
        }

        auto model = this->xmlList()->model();
        for(auto xmlFile : qAsConst(model)){
            if(!files.contains(xmlFile->file())){
                this->xmlList()->removeOne(xmlFile);
                xmlFile->deleteLater();
            }
        }

        for(const auto &file : qAsConst(files)){
            if(this->xmlList()->containsFile(file))
                continue;
            auto path = QString("%1/%2").arg(dir.absolutePath(),file);
            QFileInfo info(path);
            auto extension = info.suffix().toLower();
            XmlFile *xml = extension=="p7m" ? (new XmlP7mFile(this)) : (new XmlFile(this));
            xml->setPath(path);
            xml->setFile(file);
            this->xmlList()->append(xml);
        }

    };
    connect(this, &Archivio::showAllChanged, this, updateList);
    connect(this, &Archivio::filterFromDateChanged, this, updateList);
    connect(this, &Archivio::filterToDateChanged, this, updateList);
    connect(this, &Archivio::xmlFolderChanged, this, updateList);
    connect(this->watcher(), &QFileSystemWatcher::directoryChanged, this, updateList);
    updateList();
}
