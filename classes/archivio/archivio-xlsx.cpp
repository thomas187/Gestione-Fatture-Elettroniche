#include "archivio.h"

#include <QDesktopServices>

#include "xlsxdocument.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
using namespace QXlsx;

void Archivio::xlsxExport(QString folder)
{
    folder = folder.remove("file:///");
    QString filename = QString("export_%1.xlsx").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss"));
    QString path = QString("%1/%2").arg(folder,filename);
    QFile::copy("registro.xlsx", path);

    auto document = new Document(path, this);

    auto normal = Format();
    normal.setPatternBackgroundColor(QColor("#F2F2F2"));

    auto bold = Format();
    bold.setFontBold(true);
    bold.setPatternBackgroundColor(QColor("#F2F2F2"));

    auto header = Format();
    header.setPatternBackgroundColor(QColor("#FFFFCC"));

    auto euro = Format();
    euro.setFontItalic(true);
    euro.setNumberFormat("0.00 €");
    euro.setPatternBackgroundColor(QColor("#F2F2F2"));

    enum COLONNE{
        INTESTAZIONE = 1,
        PIVA,
        TIPO_DOCUMENTO,
        IMPORTI,
        ALIQUOTA_4,
        IVA_4,
        ALIQUOTA_5,
        IVA_5,
        ALIQUOTA_10,
        IVA_10,
        ALIQUOTA_22,
        IVA_22,
        ALIQUOTA_SPESE_22,
        IVA_SPESE_22,
    };

    auto nRows = 35;
    for(int i=0; i<this->xmlList()->count(); i++){
        auto item = this->xmlList()->get(i);
        auto sheetIndex = i/nRows+1;
        QString sheetName = QString("Foglio %1").arg(sheetIndex);
        auto row = i%nRows+2;

        bool renameSheet = sheetIndex==1 && !document->sheetNames().isEmpty();
        bool newSheet = !document->sheetNames().contains(sheetName);
        bool emptySheet = newSheet || renameSheet;
        if(renameSheet)
            document->renameSheet(document->sheetNames().at(0), sheetName);
        else if(newSheet)
            document->addSheet(sheetName);

        document->sheet(sheetName);

        if(emptySheet){
            document->setRowFormat(1,header);
            document->write(1, INTESTAZIONE, "Intestazione");
            document->write(1, PIVA, "P.IVA");
            document->write(1, TIPO_DOCUMENTO, "Tipo Documento");
            document->write(1, IMPORTI, "Tot. Documento");
            document->write(1, ALIQUOTA_4, "Imponibile (4%)");
            document->write(1, IVA_4, "Imposta (4%)");
            document->write(1, ALIQUOTA_5, "Imponibile (5%)");
            document->write(1, IVA_5, "Imposta (5%)");
            document->write(1, ALIQUOTA_10, "Imponibile (10%)");
            document->write(1, IVA_10, "Imposta (10%)");
            document->write(1, ALIQUOTA_22, "Imponibile (22%)");
            document->write(1, IVA_22, "Imposta (22%)");
            document->write(1, ALIQUOTA_SPESE_22, "Spese (22%)");
            document->write(1, IVA_SPESE_22, "Spese (22%)");

            auto rowMin = 2;
            auto rowMax = nRows+1;

            QString formula = QString("=SUM(%1%2:%1%3)");  /// =SUM(D2:D36)+'Foglio 1'!D37
            if(sheetIndex>1){
                formula += QString("+'Foglio ");
                formula += QString::number(sheetIndex-1);
                formula += "'!%1";
                formula += "%4";
            } else
                formula += "%4";

            QString arg4 = sheetIndex>1 ? QString::number(nRows+2) : "";
            document->write(nRows+2, INTESTAZIONE,      "-", normal);
            document->write(nRows+2, PIVA,              "-", normal);
            document->write(nRows+2, TIPO_DOCUMENTO,    "Tot.", bold);
            document->write(nRows+2, IMPORTI,           formula.arg(QChar((short)64+IMPORTI).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4), euro);
            document->write(nRows+2, ALIQUOTA_4,        formula.arg(QChar((short)64+ALIQUOTA_4).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4), euro);
            document->write(nRows+2, IVA_4,             formula.arg(QChar((short)64+IVA_4).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4), euro);
            document->write(nRows+2, ALIQUOTA_5,        formula.arg(QChar((short)64+ALIQUOTA_5).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4), euro);
            document->write(nRows+2, IVA_5,             formula.arg(QChar((short)64+IVA_5).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4), euro);
            document->write(nRows+2, ALIQUOTA_10,       formula.arg(QChar((short)64+ALIQUOTA_10).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4), euro);
            document->write(nRows+2, IVA_10,            formula.arg(QChar((short)64+IVA_10).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4), euro);
            document->write(nRows+2, ALIQUOTA_22,       formula.arg(QChar((short)64+ALIQUOTA_22).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4), euro);
            document->write(nRows+2, IVA_22,            formula.arg(QChar((short)64+IVA_22).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4), euro);
            document->write(nRows+2, ALIQUOTA_SPESE_22, formula.arg(QChar((short)64+ALIQUOTA_SPESE_22).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4), euro);
            document->write(nRows+2, IVA_SPESE_22,      formula.arg(QChar((short)64+IVA_SPESE_22).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4), euro);
        }

        document->write(row, INTESTAZIONE, item->intestazione(), euro);
        document->write(row, PIVA, item->partIva(), euro);
        document->write(row, TIPO_DOCUMENTO, item->tipoStringa(), euro);
        document->write(row, IMPORTI, item->totale(), euro);
        document->write(row, ALIQUOTA_4, item->imponibile_4(), euro);
        document->write(row, IVA_4, item->imposta_4(), euro);
        document->write(row, ALIQUOTA_5, item->imponibile_5(), euro);
        document->write(row, IVA_5, item->imposta_5(), euro);
        document->write(row, ALIQUOTA_10, item->imponibile_10(), euro);
        document->write(row, IVA_10, item->imposta_10(), euro);
        document->write(row, ALIQUOTA_SPESE_22, item->tipo()==XmlFile::SPESE ? item->imponibile_22() : 0, euro);
        document->write(row, IVA_SPESE_22, item->tipo()==XmlFile::SPESE ? item->imposta_22() : 0, euro);
        document->write(row, ALIQUOTA_22, item->tipo()==XmlFile::SPESE ? 0 : item->imponibile_22(), euro);
        document->write(row, IVA_22, item->tipo()==XmlFile::SPESE ? 0 : item->imposta_22(), euro);
    }

    auto sheetNames = document->sheetNames();
    if(!sheetNames.isEmpty())
        document->sheet(sheetNames.first());

    document->save();
    document->deleteLater();
    QDesktopServices::openUrl(QUrl(path));
}
