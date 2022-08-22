#ifndef ARCHIVIO_H
#define ARCHIVIO_H

#include <QObject>
#include <XmlList>
#include <XmlP7mFile>
#include <RepoMacro>
#include <QFileSystemWatcher>

class Archivio : public QObject
{
    Q_OBJECT
    Q_STATIC(Archivio)
    Q_ADD_PROPERTY(QString, xmlFolder, XmlFolder, "C:\\Users\\denic\\AppData\\Roaming\\e-media\\SDIPEC\\dati\\ricevute")
    Q_ADD_PROPERTY(XmlList*, xmlList, XmlList, nullptr)
    Q_ADD_PROPERTY(QFileSystemWatcher*, watcher, Watcher, nullptr)
    Q_ADD_PROPERTY(QStringList, spese, Spese, {})

public:
    explicit Archivio(QObject *parent = nullptr);
    ~Archivio() override;

private:
    void init();
    void initSettings();
    void initList();
    void initWatcher();
    void exportFile(QString format, QList<XmlFile*> list, QString folder, QDateTime dt);

public:
    Q_INVOKABLE void csvExport(QString folder);
    Q_INVOKABLE void xlsxExport(QString folder);
};

#endif // ARCHIVIO_H
