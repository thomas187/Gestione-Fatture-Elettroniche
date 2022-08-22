#include "xml-file.h"

QString XmlFile::find(QString find)
{
    QString value = "";

    QStringList paths = {
        "FatturaElettronica/FatturaElettronicaHeader/CedentePrestatore",
        "FatturaElettronica/FatturaElettronicaHeader/DatiTrasmissione"
    };
    QString key1 = QString("%1/%2").arg(paths.at(0), find);
    QString key2 = QString("%1/%2").arg(paths.at(1), find);

    if(this->map().keys().contains(key1))
        value = this->map().value(key1);
    else if(this->map().keys().contains(key2))
        value = this->map().value(key2);
    return value;
}

void XmlFile::getPartIva(){

    QString key_paese = "DatiAnagrafici/IdFiscaleIVA/IdPaese";
    QString key_codice = "DatiAnagrafici/IdFiscaleIVA/IdCodice";

    auto paese = this->find(key_paese);
    auto codice = this->find(key_codice);

    auto partIva = QString("%1-%2").arg(paese,codice);
    if(partIva=="-")
        qDebug() << "Impossibile trovare partIva: " << this->path();
    this->setPartIva(partIva);
}

void XmlFile::getIntestazione()
{
    auto key_intestazione = "DatiAnagrafici/Anagrafica/Denominazione";
    auto key_nome = "DatiAnagrafici/Anagrafica/Nome";
    auto key_cognome = "DatiAnagrafici/Anagrafica/Cognome";

    auto intestazione = this->find(key_intestazione);

    auto nome = this->find(key_nome);
    auto cognome = this->find(key_cognome);
    if(nome!="" && cognome!=""){
        auto nome_completo = QString("%1 %2").arg(cognome, nome);
        intestazione = nome_completo;
    }

    if(intestazione.isEmpty())
        qDebug() << "Impossibile trovare intestazione: " << this->path();
    this->setIntestazione(intestazione);
}

void XmlFile::getData(){
    auto key_data = "FatturaElettronica/FatturaElettronicaBody/DatiGenerali/DatiGeneraliDocumento/Data";
    auto data = this->map().value(key_data);
    if(data.isEmpty())
        qDebug() << "Impossibile trovare data: " << this->path();
    this->setVData(data);
}

void XmlFile::getTotale(){
    auto key_data = "FatturaElettronica/FatturaElettronicaBody/DatiGenerali/DatiGeneraliDocumento/ImportoTotaleDocumento";
    auto tot = this->map().value(key_data);
    if(tot.isEmpty())
        qDebug() << "Impossibile trovare totale: " << this->path();
    this->setTotale(tot.toDouble());
}

#include <Archivio>
void XmlFile::getTipo(){
    if(this->partIva().isEmpty())
        this->getPartIva();

    if(Archivio::getInstance().spese().contains(this->partIva())){
        this->setTipo(XmlFile::SPESE);
        return;
    }

    auto key_data = "FatturaElettronica/FatturaElettronicaBody/DatiGenerali/DatiGeneraliDocumento/TipoDocumento";
    auto tipo = this->map().value(key_data);
    if(tipo.isEmpty())
        qDebug() << "Impossibile trovare totale: " << this->path();
    this->setTipo(this->tipoFromString(tipo));
}

void XmlFile::getImposte(){
    bool _check = false;

    QString aliquota_property;

    for(int i=0; i<this->list().count(); i++){
        auto item = this->list().at(i);

        bool check_aliquota = item.first.contains("DatiRiepilogo/AliquotaIVA");
        bool check_importo = item.first.contains("DatiRiepilogo/ImponibileImporto");
        bool check_imposta = item.first.contains("DatiRiepilogo/Imposta");
        bool check_item = false;
        check_item |= check_aliquota;
        check_item |= check_importo;
        check_item |= check_imposta;
        _check |= check_item;

        if(check_aliquota){
            if(item.second.toDouble()==0.0)
                aliquota_property = "0";
            else if(item.second.toDouble()==4.0)
                aliquota_property = "4";
            else if(item.second.toDouble()==5.0)
                aliquota_property = "5";
            else if(item.second.toDouble()==22.0)
                aliquota_property = "22";
            else if(item.second.toDouble()==10.0)
                aliquota_property = "10";
            else
                qDebug() << "Impossibile definire aliquota: " << item.second << ", file:" << this->path();
        }

        if((check_importo || check_imposta) && aliquota_property.isEmpty()){
            qDebug() << "Impossibile estrarre imponibile: " << this->path();
            continue;
        }

        if(check_importo || check_imposta){
            auto key = check_importo ? "imponibile" : "imposta";
            auto property = QString("%1_%2").arg(key,aliquota_property);
            this->setProperty(property.toStdString().c_str(), item.second);
        }

        if(check_imposta)
            aliquota_property.clear();

    }

    if(!_check)
        qDebug() << "Impossibile trovare imponibile: " << this->path();
}
