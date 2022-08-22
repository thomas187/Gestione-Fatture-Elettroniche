#include "xml-list.h"

XmlList::XmlList(QObject *parent) :
    AbstractListModel(parent),
    m_proxy(new SortFilterProxyModel(this))
{
    this->init();
}

XmlList::~XmlList()
{
    auto timers = this->findChildren<QTimer*>();
    for(auto timer : qAsConst(timers))
        timer->deleteLater();
    this->clearSingleShot();
    this->clear();
    m_proxy->deleteLater();
}

void XmlList::init()
{
    this->initParsing();
    this->initProxy();
}

void XmlList::initProxy()
{
    this->proxy()->setSourceModel(this);
    this->proxy()->setSortRole(VDataRole);
    this->proxy()->sort(0, Qt::SortOrder::AscendingOrder);


    auto updateFilterRole = [=]{
        this->proxy()->setFilterRole(this->filterRole());
    };
    connect(this, &XmlList::filterRoleChanged, this, updateFilterRole);
    updateFilterRole();

    auto updateFilterString = [=]{
        this->proxy()->setFilterFixedString(XmlList::fix(this->filterString()));
    };
    connect(this, &XmlList::filterStringChanged, this, updateFilterString);
    updateFilterString();


}

QString XmlList::fix(QString string)
{
    string = string.toLower();
    string = string.remove(QRegExp("[^a-zA-Z0-9]+"));
    return string;
}

void XmlList::initParsing()
{

    auto syncFile = [=]{
        auto currentIndex = -1;

        for(int i=0; i<this->count(); i++){
            auto file = this->get(i);
            if(!file->sync()){
                currentIndex = i;
                break;
            }
        }

        if(currentIndex<0)
            return;

        auto perc = (double)currentIndex/(double)(this->count()-1);
        if(perc>1.0)
            perc = 1.0;
        if(perc<0)
            perc = 0.0;
        this->setLoading(perc);

        auto file = this->get(currentIndex);
        file->parseXmlFile();
    };

    auto startStopTimers = [=]{
        auto timers = this->findChildren<QTimer*>();
        for(auto timer : qAsConst(timers))
            timer->deleteLater();

        if(this->isEmpty())
            return;

        singleShot("startStopTimers", 5, this, [=](QVariant, int, QObject*){
            auto timer = new QTimer(this);
            timer->setInterval(5);
            connect(timer, &QTimer::timeout, this, syncFile);
            syncFile();
        });
    };
    connect(this, &XmlList::modelChanged, this, startStopTimers);
    startStopTimers();
}
