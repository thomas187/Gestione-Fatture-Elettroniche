#include "archivio.h"

#include <QDesktopServices>

#include "xlsxdocument.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
using namespace QXlsx;

void Archivio::xlsxExport(QString folder, QString expFromStrDate, QString expToStrDate)
{
    QDate expFromDate = QDate::fromString(expFromStrDate, "yyyy-MM-dd");
    QDate expToDate = QDate::fromString(expToStrDate, "yyyy-MM-dd");
    expToDate = expToDate.addMonths(1);

    folder = folder.remove("file:///");
    QString filename = QString("export_%1.xlsx").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss"));
    QString path = QString("%1/%2").arg(folder,filename);
    QFile::copy("registro.xlsx", path);

    auto document = new Document(path, this);

    auto font = QFont();
    font.setPointSize(14);

    auto bold = Format();
    bold.setFont(font);
    bold.setPatternBackgroundColor(QColor(242,242,242));
    bold.setFontBold(true);
    bold.setHorizontalAlignment(Format::HorizontalAlignment::AlignHCenter);
    bold.setVerticalAlignment(Format::VerticalAlignment::AlignVCenter);
    bold.setBottomBorderColor(QColor(196,189,151));
    bold.setBottomBorderStyle(Format::BorderDashDot);
    bold.setRightBorderColor(QColor(0,0,0));
    bold.setRightBorderStyle(Format::BorderThin);

    auto normal = Format();
    normal.setFont(font);
    normal.setHorizontalAlignment(Format::HorizontalAlignment::AlignHCenter);
    normal.setVerticalAlignment(Format::VerticalAlignment::AlignVCenter);
    normal.setBottomBorderColor(QColor(196,189,151));
    normal.setBottomBorderStyle(Format::BorderDashDot);
    normal.setRightBorderColor(QColor(0,0,0));
    normal.setRightBorderStyle(Format::BorderThin);

    auto spese = Format();
    spese.setFont(font);
    spese.setPatternBackgroundColor(QColor(242,220,219));
    spese.setHorizontalAlignment(Format::HorizontalAlignment::AlignHCenter);
    spese.setVerticalAlignment(Format::VerticalAlignment::AlignVCenter);
    spese.setBottomBorderColor(QColor(196,189,151));
    spese.setBottomBorderStyle(Format::BorderDashDot);
    spese.setRightBorderColor(QColor(0,0,0));
    spese.setRightBorderStyle(Format::BorderThin);

    auto note = Format();
    note.setFont(font);
    note.setPatternBackgroundColor(QColor(235,241,222));
    note.setHorizontalAlignment(Format::HorizontalAlignment::AlignHCenter);
    note.setVerticalAlignment(Format::VerticalAlignment::AlignVCenter);
    note.setBottomBorderColor(QColor(196,189,151));
    note.setBottomBorderStyle(Format::BorderDashDot);
    note.setRightBorderColor(QColor(0,0,0));
    note.setRightBorderStyle(Format::BorderThin);

    enum COLONNE{
        N_OPERAZ = 1,
        DATA,
        PIVA,
        INTESTAZIONE,
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
        ALTRO
    };

    auto inserisciValoreDocumento = [=](XmlFile *item, int row, int column, double value, Format format) {
        Q_ASSERT_X(item!=nullptr, Q_FUNC_INFO, "Item non definito");
        value = item->notaDiCredito() ? -value : value;
        document->write(row, column, value, format);
    };

    auto dummySheet = document->sheetNames().count()>0 ? document->sheetNames().at(0) : "";

    auto nRows = 35;
    auto rowMin = 3;
    auto rowMax = nRows+2;

    QList<XmlFile*> items;
    QSortFilterProxyModel sorted;
    sorted.setSourceModel(this->xmlList());
    sorted.setSortRole(XmlList::VDataRole);
    sorted.sort(0);
    for(int i=0; i<sorted.rowCount(); i++){
        auto index = sorted.index(i,0);
        auto sourceIndex = sorted.mapToSource(index);
        auto item = this->xmlList()->get(sourceIndex.row());
        if(item->date()<expFromDate || item->date()>expToDate)
            continue;
        items += item;
    }

    if(items.isEmpty())
        return;

    auto rest = 35*((items.count()/35)+1);
    for(int i=0; i<items.count(); i++){
        auto item = items.at(i);
        auto sheetIndex = i/nRows+1;
        QString sheetName = QString("Foglio %1").arg(sheetIndex);
        auto row = i%nRows+rowMin;

        bool newSheet = !document->sheetNames().contains(sheetName);
        if(newSheet){
            if(dummySheet.isEmpty())
                document->addSheet(sheetName);
            else
                document->copySheet(dummySheet,sheetName);
        }
        document->selectSheet(sheetName);
        document->setRowHeight(row,30);

        if(newSheet){
            QString formula = QString("=SUM(%1%2:%1%3)");  /// =SUM(D2:D36)+'Foglio 1'!D37
            if(sheetIndex>1){
                formula += QString("+'Foglio ");
                formula += QString::number(sheetIndex-1);
                formula += "'!%1";
                formula += "%4";
            } else
                formula += "%4";

            QString arg4 = sheetIndex>1 ? QString::number(nRows+rowMin) : "";
            document->setRowHeight(nRows+rowMin,30);
            document->write(nRows+rowMin, IMPORTI,           formula.arg(QChar((short)64+IMPORTI).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4));
            document->write(nRows+rowMin, ALIQUOTA_4,        formula.arg(QChar((short)64+ALIQUOTA_4).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4));
            document->write(nRows+rowMin, IVA_4,             formula.arg(QChar((short)64+IVA_4).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4));
            document->write(nRows+rowMin, ALIQUOTA_5,        formula.arg(QChar((short)64+ALIQUOTA_5).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4));
            document->write(nRows+rowMin, IVA_5,             formula.arg(QChar((short)64+IVA_5).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4));
            document->write(nRows+rowMin, ALIQUOTA_10,       formula.arg(QChar((short)64+ALIQUOTA_10).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4));
            document->write(nRows+rowMin, IVA_10,            formula.arg(QChar((short)64+IVA_10).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4));
            document->write(nRows+rowMin, ALIQUOTA_22,       formula.arg(QChar((short)64+ALIQUOTA_22).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4));
            document->write(nRows+rowMin, IVA_22,            formula.arg(QChar((short)64+IVA_22).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4));
            document->write(nRows+rowMin, ALIQUOTA_SPESE_22, formula.arg(QChar((short)64+ALIQUOTA_SPESE_22).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4));
            document->write(nRows+rowMin, IVA_SPESE_22,      formula.arg(QChar((short)64+IVA_SPESE_22).toLatin1()).arg(rowMin).arg(rowMax).arg(arg4));
        }

        auto boldFormat = bold;
        auto format = normal;

        if(item->tipo()==XmlFile::SPESE){
            boldFormat.setPatternBackgroundColor(spese.patternBackgroundColor());
            format = spese;
        }
        if(item->notaDiCredito()){
            boldFormat.setPatternBackgroundColor(note.patternBackgroundColor());
            format = note;
        }

        document->write(row, N_OPERAZ, i+1, boldFormat);

        document->write(row, DATA, item->date().toString("yyyy-MM-dd"), boldFormat);

        document->write(row, PIVA, item->partIva(), format);

        auto iLength = item->intestazione().length();
        auto intestazione = iLength > 30 ? item->intestazione().left(28)+"…" : item->intestazione();
        auto intestazione_format = format;
        intestazione_format.setHorizontalAlignment(Format::HorizontalAlignment::AlignLeft);
        normal.setHorizontalAlignment(Format::HorizontalAlignment::AlignHCenter);
        document->write(row, INTESTAZIONE, intestazione, intestazione_format);

        auto tLength = item->tipoStringa().length();
        auto tipo = tLength > 15 ? item->tipoStringa().left(13)+"…" : item->tipoStringa();
        document->write(row, TIPO_DOCUMENTO, tipo, format);

        inserisciValoreDocumento(item, row, IMPORTI, item->totale(), format);
        inserisciValoreDocumento(item, row, ALIQUOTA_4, item->imponibile_4(), format);
        inserisciValoreDocumento(item, row, IVA_4, item->imposta_4(), format);
        inserisciValoreDocumento(item, row, ALIQUOTA_5, item->imponibile_5(), format);
        inserisciValoreDocumento(item, row, IVA_5, item->imposta_5(), format);
        inserisciValoreDocumento(item, row, ALIQUOTA_10, item->imponibile_10(), format);
        inserisciValoreDocumento(item, row, IVA_10, item->imposta_10(), format);
        inserisciValoreDocumento(item, row, ALIQUOTA_SPESE_22, item->tipo()==XmlFile::SPESE ? item->imponibile_22() : 0, format);
        inserisciValoreDocumento(item, row, IVA_SPESE_22, item->tipo()==XmlFile::SPESE ? item->imposta_22() : 0, format);
        inserisciValoreDocumento(item, row, ALIQUOTA_22, item->tipo()==XmlFile::SPESE ? 0 : item->imponibile_22(), format);
        inserisciValoreDocumento(item, row, IVA_22, item->tipo()==XmlFile::SPESE ? 0 : item->imposta_22(), format);

        auto altro_format = format;
        altro_format.setHorizontalAlignment(Format::HorizontalAlignment::AlignLeft);
        normal.setHorizontalAlignment(Format::HorizontalAlignment::AlignHCenter);
        document->write(row, ALTRO, QString(), altro_format);

    }

    for(int i=items.count(); i<rest; i++){
        auto sheetIndex = i/nRows+1;
        QString sheetName = QString("Foglio %1").arg(sheetIndex);
        auto row = i%nRows+rowMin;
        document->selectSheet(sheetName);
        document->setRowHeight(row,30);

        auto boldFormat = bold;
        auto format = normal;

        document->write(row, N_OPERAZ, i+1, boldFormat);
        document->write(row, DATA, "", boldFormat);
        document->write(row, PIVA, "", format);
        document->write(row, INTESTAZIONE, "", format);
        document->write(row, TIPO_DOCUMENTO, "", format);
        document->write(row, IMPORTI, "", format);
        document->write(row, ALIQUOTA_4, "", format);
        document->write(row, IVA_4, "", format);
        document->write(row, ALIQUOTA_5, "", format);
        document->write(row, IVA_5, "", format);
        document->write(row, ALIQUOTA_10, "", format);
        document->write(row, IVA_10, "", format);
        document->write(row, ALIQUOTA_SPESE_22, "", format);
        document->write(row, IVA_SPESE_22, "", format);
        document->write(row, ALIQUOTA_22, "", format);
        document->write(row, IVA_22, "", format);
        document->write(row, ALTRO, "", format);

    }

    auto sheetNames = document->sheetNames();
    for(const auto &sheet : qAsConst(sheetNames)){
        document->selectSheet(sheet);

        document->setColumnWidth(N_OPERAZ,15);
        document->setColumnWidth(DATA,18);
        document->setColumnWidth(PIVA,22);
        document->setColumnWidth(INTESTAZIONE,43);
        document->setColumnWidth(TIPO_DOCUMENTO,20);
        document->setColumnWidth(ALTRO,30);
        for(int c=IMPORTI; c<=IVA_SPESE_22; c++)
            document->setColumnWidth(c,18);
    }

    if(!dummySheet.isEmpty() && sheetNames.contains(dummySheet))
        document->deleteSheet(dummySheet);
    if(!sheetNames.isEmpty())
        document->selectSheet(sheetNames.first());

    document->save();
    document->deleteLater();
    QDesktopServices::openUrl(QUrl(path));
}
