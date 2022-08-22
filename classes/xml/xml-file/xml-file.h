#ifndef XML_FILE_H
#define XML_FILE_H

#include <QObject>
#include <RepoMacro>
#include <QDate>
#include <QFile>
#include <QXmlStreamReader>

class XmlFile : public QObject
{
    Q_OBJECT
    Q_ADD_PROPERTY(QString, path, Path, "")
    Q_ADD_PROPERTY(QString, partIva, PartIva, "")
    Q_ADD_PROPERTY(QString, intestazione, Intestazione, "")
    Q_ADD_PROPERTY(QString, vData, VData, "")
    Q_ADD_PROPERTY_DOUBLE(double, imponibile_4, Imponibile_4, 0)
    Q_ADD_PROPERTY_DOUBLE(double, imposta_4, Imposta_4, 0)
    Q_ADD_PROPERTY_DOUBLE(double, imponibile_5, Imponibile_5, 0)
    Q_ADD_PROPERTY_DOUBLE(double, imposta_5, Imposta_5, 0)
    Q_ADD_PROPERTY_DOUBLE(double, imponibile_10, Imponibile_10, 0)
    Q_ADD_PROPERTY_DOUBLE(double, imposta_10, Imposta_10, 0)
    Q_ADD_PROPERTY_DOUBLE(double, imponibile_22, Imponibile_22, 0)
    Q_ADD_PROPERTY_DOUBLE(double, imposta_22, Imposta_22, 0)
    Q_ADD_PROPERTY_DOUBLE(double, totale, Totale, 0)
    Q_ADD_PROPERTY_MAP(QString, QString, map, Map)

public:
    enum DOCUMENT_TYPE{
        NONE,
        SPESE,/// Spese al 22%
        RT01, /// Ritenuta persone fisiche
        RT02, /// Ritenuta persone giuridiche
        RT03, /// Contributo INPS
        RT04, /// Contributo ENASARCO
        RT05, /// Contributo ENPAM
        RT06, /// Altro contributo previdenziale
        TD01, /// Fattura
        TD02, /// Acconto/Anticipo su fattura
        TD03, /// Acconto/Anticipo su parcella
        TD04, /// Nota di Credito
        TD05, /// Nota di Debito
        TD06, /// Parcella
        TD07, /// Fattura semplificata
        TD08, /// Nota di Credito semplificata
        TD16, /// Integrazione fattura reverse charge interno
        TD17, /// Integrazione/autofattura per acquisto servizi dall’estero
        TD18, /// Integrazione per acquisto di beni intracomunitari
        TD19, /// Integrazione/autofattura per acquisto di beni
        TD20, /// Autofattura per regolarizzazione e integrazione delle fatture
        TD21, /// Autofattura per splafonamento
        TD22, /// Estrazione beni da Deposito IVA
        TD23, /// Estrazione beni da Deposito IVA con versamento dell’IVA
        TD24, /// Fattura differita di cui all’art. 21, comma 4, lett. a)
        TD25, /// Fattura differita di cui all’art. 21, comma 4, terzo periodo lett. b)
        TD26, /// Cessione di beni ammortizzabili e per passaggi interni (ex art. 36 DPR n. 633/1972)
        TD27, /// Fattura per autoconsumo o per cessioni gratuite senza rivalsa
    }; Q_ENUM(DOCUMENT_TYPE)
    Q_ADD_PROPERTY(DOCUMENT_TYPE, tipo, Tipo, NONE)
    Q_ADD_PROPERTY(QString, tipoStringa, TipoStringa, "Non definito")

public:
    enum SYNC_STATUS{
        OFF,
        LOADING,
        ERROR,
        READY
    }; Q_ENUM(SYNC_STATUS)
    Q_ADD_PROPERTY(SYNC_STATUS, sync, Sync, OFF)

public:
    typedef QPair<QString,QString> CoupleString;
    Q_ADD_PROPERTY_LIST(CoupleString, list, List, {})

public:
    explicit XmlFile(QObject *parent = nullptr);

    operator QString() const {
        return this->toString();
    }

    QString toString() const{
        return QString("%1 - %2")
               .arg(this->vData(),this->intestazione());
    }

public:
    void parseXmlFile();
    QList<QPair<QString, QString>> getList(QString xmlData);
    void createMap();

protected:
    virtual QString openData();

private:
    void init();
    void initTipoStringa();
    QString find(QString find);
    void getPartIva();
    void getIntestazione();
    void getData();
    void getImposte();
    void getTotale();
    void getTipo();
    void checkSync();
    XmlFile::DOCUMENT_TYPE tipoFromString(QString value);
};

#endif // XML_FILE_H
