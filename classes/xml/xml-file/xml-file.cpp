#include "xml-file.h"

XmlFile::XmlFile(QObject *parent)
    : QObject{parent}
{
    this->init();
}

void XmlFile::init()
{
    this->initTipoStringa();
    this->initDate();
}

void XmlFile::initDate()
{
    auto updateDate = [=]{
        this->setDate(QDate::fromString(this->vData(), "yyyy-MM-dd"));
    };
    connect(this, &XmlFile::vDataChanged, this, updateDate);
    updateDate();
}

void XmlFile::initTipoStringa()
{
    auto updateTipoStringa = [=]{
        QString tipoStringa = "-";
        switch (this->tipo()) {
            case RT01:
                tipoStringa = "Ritenuta persone fisiche";
                break;
            case RT02:
                tipoStringa = "Ritenuta persone giuridiche";
                break;
            case RT03:
                tipoStringa = "Contributo INPS";
                break;
            case RT04:
                tipoStringa = "Contributo ENASARCO";
                break;
            case RT05:
                tipoStringa = "Contributo ENPAM";
                break;
            case RT06:
                tipoStringa = "Altro contributo previdenziale ";
                break;
            case TD01:
                tipoStringa = "Fattura";
                break;
            case TD02:
                tipoStringa = "Acconto/Anticipo su fattura";
                break;
            case TD03:
                tipoStringa = "Acconto/Anticipo su parcella";
                break;
            case TD04:
                tipoStringa = "Nota di Credito";
                break;
            case TD05:
                tipoStringa = "Nota di Debito";
                break;
            case TD06:
                tipoStringa = "Parcella";
                break;
            case TD07:
                tipoStringa = "Fattura semplificata";
                break;
            case TD08:
                tipoStringa = "Nota di Credito semplificata";
                break;
            case TD16:
                tipoStringa = "Integrazione fattura reverse charge interno";
                break;
            case TD17:
                tipoStringa = "Integrazione/autofattura per acquisto servizi dall’estero";
                break;
            case TD18:
                tipoStringa = "Integrazione per acquisto di beni intracomunitari";
                break;
            case TD19:
                tipoStringa = "Integrazione/autofattura per acquisto di beni";
                break;
            case TD20:
                tipoStringa = "Autofattura per regolarizzazione e integrazione delle fatture";
                break;
            case TD21:
                tipoStringa = "Autofattura per splafonamento";
                break;
            case TD22:
                tipoStringa = "Estrazione beni da Deposito IVA";
                break;
            case TD23:
                tipoStringa = "Estrazione beni da Deposito IVA con versamento dell’IVA";
                break;
            case TD24:
                tipoStringa = "Fattura differita di cui all’art. 21, comma 4, lett. a)";
                break;
            case TD25:
                tipoStringa = "Fattura differita di cui all’art. 21, comma 4, terzo periodo lett. b)";
                break;
            case TD26:
                tipoStringa = "Cessione di beni ammortizzabili e per passaggi interni (ex art. 36 DPR n. 633/1972)";
                break;
            case TD27:
                tipoStringa = "Fattura per autoconsumo o per cessioni gratuite senza rivalsa";
                break;
            case SPESE:
                tipoStringa = "Spese al 22%";
                break;
            default:
                tipoStringa = "Non definito";
                break;
        }
        this->setTipoStringa(tipoStringa);
    };
    connect(this, &XmlFile::tipoChanged, this, updateTipoStringa);
    updateTipoStringa();
}

void XmlFile::parseXmlFile()
{
    this->setSync(XmlFile::LOADING);
    auto xmlData = this->openData();
    auto list = this->getList(xmlData);
    this->setList(list);
    this->createMap();
    this->getPartIva();
    this->getIntestazione();
    this->getData();
    this->getImposte();
    this->getTotale();
    this->getTipo();
    this->checkSync();
}

void XmlFile::checkSync()
{
    bool check = true;
    check &= this->path()!="";
    check &= this->partIva()!="";
    check &= this->intestazione()!="";
    check &= this->vData()!="";

    bool check_imposte = false;
    check_imposte |= (this->imponibile_4()>0 && this->imposta_4()>0);
    check_imposte |= (this->imponibile_5()>0 && this->imposta_5()>0);
    check_imposte |= (this->imponibile_10()>0 && this->imposta_10()>0);
    check_imposte |= (this->imponibile_22()>0 && this->imposta_22()>0);
    check &= check_imposte;

    check &= this->totale()>0;

    this->setSync(check ? READY : ERROR);
}

XmlFile::DOCUMENT_TYPE XmlFile::tipoFromString(QString value)
{
    if(value=="RT01") return XmlFile::RT01;
    else if(value=="RT02") return XmlFile::RT02;
    else if(value=="RT03") return XmlFile::RT03;
    else if(value=="RT04") return XmlFile::RT04;
    else if(value=="RT05") return XmlFile::RT05;
    else if(value=="RT06") return XmlFile::RT06;
    else if(value=="TD01") return XmlFile::TD01;
    else if(value=="TD02") return XmlFile::TD02;
    else if(value=="TD03") return XmlFile::TD03;
    else if(value=="TD04") return XmlFile::TD04;
    else if(value=="TD05") return XmlFile::TD05;
    else if(value=="TD06") return XmlFile::TD06;
    else if(value=="TD07") return XmlFile::TD07;
    else if(value=="TD08") return XmlFile::TD08;
    else if(value=="TD16") return XmlFile::TD16;
    else if(value=="TD17") return XmlFile::TD17;
    else if(value=="TD18") return XmlFile::TD18;
    else if(value=="TD19") return XmlFile::TD19;
    else if(value=="TD20") return XmlFile::TD20;
    else if(value=="TD21") return XmlFile::TD21;
    else if(value=="TD22") return XmlFile::TD22;
    else if(value=="TD23") return XmlFile::TD23;
    else if(value=="TD24") return XmlFile::TD24;
    else if(value=="TD25") return XmlFile::TD25;
    else if(value=="TD26") return XmlFile::TD26;
    else if(value=="TD27") return XmlFile::TD27;
    else return XmlFile::NONE;
}
