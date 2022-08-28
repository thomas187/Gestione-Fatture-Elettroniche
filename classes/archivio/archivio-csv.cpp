#include "archivio.h"
#include <QDesktopServices>

void Archivio::exportFile(QString format, QList<XmlFile*> list, QString folder, QDateTime dt)
{
    folder = folder.remove("file:///");
    QString filename = QString(format).arg(dt.toString("yyyy-MM-dd_hh-mm-ss"));
    QString path = QString("%1/%2").arg(folder,filename);
    QFile fileData(path);
    if(!fileData.open(QIODevice::ReadWrite | QIODevice::Append ))
        return;
    fileData.resize(0);
    QTextStream outData(&fileData);

    QList<int> imps({4,5,10,22});
    QStringList header({"Partita IVA","Tipo documento","Intestazione","Data","Totale fattura"});
    for(auto imp : qAsConst(imps)){
        header += QString("Imponibile (%1\%)").arg(imp);
        header += QString("Imposta (%1\%)").arg(imp);
    }
    header += "XML";

    QString text = header.join(";");
    text += "\n";

    for(auto item : qAsConst(list)){
        QString line;
        line += item->partIva() + ";";
        line += item->tipoStringa() + ";";
        line += item->intestazione() + ";";
        line += item->vData() + ";";
        line += QString::number(item->totale(),'f',2).replace(".",",") + ";";

        for(auto imp : qAsConst(imps)){
            auto imponibile = item->property(QString("imponibile_%1").arg(imp).toStdString().c_str()).toDouble();
            if(imponibile>0)
                line += QString::number(imponibile,'f',2).replace(".",",") + ";";
            else
                line += "-;";
            auto imposta = item->property(QString("imposta_%1").arg(imp).toStdString().c_str()).toDouble();
            if(imposta>0)
                line += QString::number(imposta,'f',2).replace(".",",") + ";";
            else
                line += "-;";
        }

        line += item->path()+"\n";
        text += line;
    }

    outData << text;
    fileData.close();
    QDesktopServices::openUrl(QUrl(path));
}

void Archivio::csvExport(QString folder, QString expFromStrDate, QString expToStrDate)
{
    QDate expFromDate = QDate::fromString(expFromStrDate, "yyyy-MM-dd");
    QDate expToDate = QDate::fromString(expToStrDate, "yyyy-MM-dd");

    QList<XmlFile*> listFatture;
    QList<XmlFile*> listSpese;
    QList<XmlFile*> listNoteCredito;

    auto list = this->xmlList()->model();
    for(auto item : qAsConst(list)){
        if(item->date()<expFromDate || item->date()>expToDate)
            continue;
        if(Archivio::getInstance().spese().contains(item->partIva()))
            listSpese += item;
        else if(item->tipo()==XmlFile::TD04 || item->tipo()==XmlFile::TD08)
            listNoteCredito += item;
        else
            listFatture += item;
    }

    auto dt = QDateTime::currentDateTime();

    if(!listFatture.isEmpty())
        this->exportFile("export_%1.csv", listFatture, folder, dt);

    if(!listNoteCredito.isEmpty())
        this->exportFile("export_note_%1.csv", listNoteCredito, folder, dt);

    if(!listSpese.isEmpty())
        this->exportFile("export_spese_%1.csv", listSpese, folder, dt);
}
