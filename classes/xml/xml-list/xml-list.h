#ifndef XML_LIST_H
#define XML_LIST_H

#include <AbstractListModel>
#include <SortFilterProxyModel>
#include <XmlFile>
#include <QList>
#include <QTimer>
#include <RepoMacro>
#include <Singleshot>
#include <QFileInfo>

class XmlList :
        public AbstractListModel,
        public Singleshot
{
    Q_OBJECT
    Q_ADD_PROPERTY_DOUBLE(double, loading, Loading, 0.0)
    Q_ADD_PROPERTY(SortFilterProxyModel*, proxy, Proxy, nullptr)

public:
    Q_ENUMS(XmlFileRoles)
    enum XmlFileRoles {
        PathRole = Qt::UserRole + 1,
        PartIvaRole,
        PartIvaFixedRole,
        IntestazioneRole,
        IntestazioneFixedRole,
        VDataRole,
        VDataFixedRole,
        DateRole,
        Imponibile_4Role,
        Imposta_4Role,
        Imponibile_5Role,
        Imposta_5Role,
        Imponibile_10Role,
        Imposta_10Role,
        Imponibile_22Role,
        Imposta_22Role,
        TotaleRole,
        FileNameRole,
        FileNameFixedRole,
        SyncRole,
        TipoRole,
        TipoStringaRole,
        TipoStringaFixedRole,
    };
    Q_ADD_PROPERTY(XmlFileRoles, filterRole, FilterRole, IntestazioneFixedRole)
    Q_ADD_PROPERTY(QString, filterString, FilterString, "")

public:
    QHash<int, QByteArray> roleNames() const override{
        QHash<int, QByteArray> roles;
        roles[PathRole] = "path";
        roles[PartIvaRole] = "partIva";
        roles[PartIvaFixedRole] = "partIvaFixed";
        roles[IntestazioneRole] = "intestazione";
        roles[IntestazioneFixedRole] = "intestazioneFixed";
        roles[VDataRole] = "vData";
        roles[VDataFixedRole] = "vDataFixed";
        roles[DateRole] = "date";
        roles[Imponibile_4Role] = "imponibile_4";
        roles[Imposta_4Role] = "imposta_4";
        roles[Imponibile_5Role] = "imponibile_5";
        roles[Imposta_5Role] = "imposta_5";
        roles[Imponibile_10Role] = "imponibile_10";
        roles[Imposta_10Role] = "imposta_10";
        roles[Imponibile_22Role] = "imponibile_22";
        roles[Imposta_22Role] = "imposta_22";
        roles[TotaleRole] = "totale";
        roles[FileNameRole] = "fileName";
        roles[FileNameFixedRole] = "fileNameFixed";
        roles[SyncRole] = "sync";
        roles[TipoRole] = "tipo";
        roles[TipoStringaRole] = "tipoStringa";
        roles[TipoStringaFixedRole] = "tipoStringaFixed";

        return roles;
    }

public:
    explicit XmlList(QObject *parent = nullptr);
    ~XmlList() override;

private:
    void init();
    void initParsing();
    void initProxy();

private:
    static QString fix(QString string);

public:
    XmlList(const XmlList &other, QObject *parent = nullptr)
        : AbstractListModel(parent) {
        m_model = other.model();
        emit modelChanged();
    }
    XmlList& operator =(const XmlList &other) {
        m_model = other.model();
        emit modelChanged();
        return *this;
    }
    XmlList& operator =(XmlList &other) {
        m_model = other.model();
        emit modelChanged();
        return *this;
    }

    bool operator == (const XmlList &other) const{
       return m_model==other.model();
    }

    void operator += (XmlFile* xmlFile){
        this->append(xmlFile);
    }

    void operator -= (XmlFile* xmlFile){
        this->removeOne(xmlFile);
    }

    operator QString() const {
        QStringList print;
        for(XmlFile *xmlFile: m_model)
            print += static_cast<QString>(*xmlFile);
        return QString("LIST[%1]")
                .arg(print.join(", "));
    }

    QList<XmlFile*> model(){
        return m_model;
    }

    QList<XmlFile*> model() const{
        return m_model;
    }

    void removeDuplicates(){
        /// Rimuovo duplicati
        std::sort(m_model.begin(), m_model.end());
        m_model.erase(std::unique(m_model.begin(), m_model.end()), m_model.end());
    }

    using AbstractListModel::QAbstractListModel;

    QHash<int,QByteArray> roleArchives() const  {
        return {
            {PathRole, "path"},
            {PartIvaRole, "partIva"},
            {PartIvaFixedRole, "partIvaFixed"},
            {IntestazioneRole, "intestazione"},
            {IntestazioneFixedRole, "intestazioneFixed"},
            {VDataRole, "vData"},
            {VDataFixedRole, "vDataFixed"},
            {DateRole, "date"},
            {Imponibile_4Role, "imponibile_4"},
            {Imposta_4Role, "imposta_4"},
            {Imponibile_5Role, "imponibile_5"},
            {Imposta_5Role, "imposta_5"},
            {Imponibile_10Role, "imponibile_10"},
            {Imposta_10Role, "imposta_10"},
            {Imponibile_22Role, "imponibile_22"},
            {Imposta_22Role, "imposta_22"},
            {TotaleRole, "totale"},
            {FileNameRole, "fileName"},
            {FileNameFixedRole, "fileNameFixed"},
            {SyncRole, "sync"},
            {TipoRole, "tipo"},
            {TipoStringaRole, "tipoStringa"},
            {TipoStringaFixedRole, "tipoStringaFixed"},
        };
    }

    void append(XmlFile* xmlFile){
        int index = this->rowCount();
        beginInsertRows(QModelIndex(), index, index);
        m_model.append(xmlFile);
        this->connectXmlFile(xmlFile);
        endInsertRows();
        emit modelChanged();
        emit rowInserted(this->index(index, 0));
    }

    void insert(int index, XmlFile *xmlFile){
        ASSERT_X(m_model.count()>index, Q_FUNC_INFO, QString("impossible to access at index %1 (count %2)")
                   .arg(index)
                   .arg(m_model.count())
                   .toStdString().c_str());
        beginInsertRows(QModelIndex(), index, index);
        m_model.insert(index, xmlFile);
        endInsertRows();
        emit modelChanged();
        emit rowInserted(this->index(index, 0));
    }

    bool contains(XmlFile *xmlFile){
        return m_model.contains(xmlFile);
    }

    bool contains(QString path){
        bool contains = false;
        for(XmlFile* file : qAsConst(m_model)){
            if(contains)
                break;
            bool itemContains = true;
            itemContains &= file->path() == path;
            contains |= itemContains;
        }
        return contains;
    }

    bool removeOne(XmlFile *xmlFile){
        ASSERT_X_RET(this->contains(xmlFile), Q_FUNC_INFO, "xmlFile out of range", false);
        int row = m_model.indexOf(xmlFile);
        beginRemoveRows(QModelIndex(), row, row);
        int ret = m_model.removeOne(xmlFile);
        this->disconnectXmlFile(xmlFile);
        endRemoveRows();
        emit modelChanged();
        return ret;
    }

    void removeAt(int row){
        ASSERT_X(row>=0, Q_FUNC_INFO, "negative index");
        ASSERT_X(m_model.count()>row, Q_FUNC_INFO, QString("list xmlFile count(%1), index(%2)")
                   .arg(m_model.count(),row).toStdString().c_str());
        beginRemoveRows(QModelIndex(), row, row);
        this->disconnectXmlFile(m_model.at(row));
        m_model.removeAt(row);
        endRemoveRows();
        emit modelChanged();
    }

    int removeAll(XmlFile *xmlFile){
        beginResetModel();
        int ret = m_model.removeAll(xmlFile);
        this->disconnectXmlFile(xmlFile);
        endResetModel();
        emit modelChanged();
        return ret;
    }

    void removeFirst(){
        ASSERT_X(!m_model.isEmpty(), Q_FUNC_INFO, "list is empty");
        int row = 0;
        beginRemoveRows(QModelIndex(), row, row);
        this->disconnectXmlFile(m_model.first());
        m_model.removeFirst();
        endRemoveRows();
        emit modelChanged();
    }

    void removeLast(){
        ASSERT_X(!m_model.isEmpty(), Q_FUNC_INFO, "list is empty");
        int row = m_model.count();
        beginRemoveRows(QModelIndex(), row, row);
        this->disconnectXmlFile(m_model.last());
        m_model.removeLast();
        endRemoveRows();
        emit modelChanged();
    }

    void clear(){
        beginResetModel();
        for(XmlFile *xmlFile : qAsConst(m_model))
            this->disconnectXmlFile(xmlFile);
        m_model.clear();
        endResetModel();
        emit modelChanged();
    }

    bool isEmpty(){
        return m_model.isEmpty();
    }

    Q_INVOKABLE XmlFile* get(int i) {
        XmlFile *xmlFile = nullptr;
        ASSERT_X_RET(i>=0, Q_FUNC_INFO, "negative index", xmlFile);
        ASSERT_X_RET(m_model.count()>i, Q_FUNC_INFO, QString("list xmlFile count(%1), index(%2)").arg(m_model.count(),i).toStdString().c_str(), xmlFile);
        xmlFile = m_model.at(i);
        return xmlFile;
    }

    XmlFile* item(int i){
        XmlFile *xmlFile = nullptr;
        ASSERT_X_RET(i>=0, Q_FUNC_INFO, "negative index", xmlFile);
        ASSERT_X_RET(m_model.count()>i, Q_FUNC_INFO, QString("list xmlFile count(%1), index(%2)").arg(m_model.count(),i).toStdString().c_str(), xmlFile);
        xmlFile = m_model.at(i);
        return xmlFile;
    }

    int rowCount(const QModelIndex & parent = QModelIndex()) const override {
        if (parent.isValid())
            return 0;
        return m_model.count();
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override
    {
        if (!hasIndex(index.row(), index.column(), index.parent()) || !value.isValid())
            return false;

        XmlFile *item = m_model.at(index.row());

        if(role == PathRole) item->setPath(value.toString());
        else if(role == PartIvaRole) item->setPartIva(value.toString());
        else if(role == PartIvaFixedRole) return false;
        else if(role == IntestazioneRole) item->setIntestazione(value.toString());
        else if(role == IntestazioneFixedRole) return false;
        else if(role == VDataRole) item->setVData(value.toString());
        else if(role == VDataFixedRole) return false;
        else if(role == DateRole) return false;
        else if(role == Imponibile_4Role) item->setImponibile_4(value.toDouble());
        else if(role == Imposta_4Role) item->setImposta_4(value.toDouble());
        else if(role == Imponibile_5Role) item->setImponibile_5(value.toDouble());
        else if(role == Imposta_5Role) item->setImposta_5(value.toDouble());
        else if(role == Imponibile_10Role) item->setImponibile_10(value.toDouble());
        else if(role == Imposta_10Role) item->setImposta_10(value.toDouble());
        else if(role == Imponibile_22Role) item->setImponibile_22(value.toDouble());
        else if(role == Imposta_22Role) item->setImposta_22(value.toDouble());
        else if(role == TotaleRole) item->setTotale(value.toDouble());
        else if(role == FileNameRole) return false;
        else if(role == FileNameFixedRole) return false;
        else if(role == SyncRole) item->setSync(value.value<XmlFile::SYNC_STATUS>());
        else if(role == TipoRole) item->setTipo(value.value<XmlFile::DOCUMENT_TYPE>());
        else if(role == TipoStringaRole) return false;
        else if(role == TipoStringaFixedRole) return false;
        else
            return false;

        emit dataChanged(index, index, { role } );
        return true ;
    }

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override {
        if (!hasIndex(index.row(), index.column(), index.parent()))
            return {};

        XmlFile *item = m_model.at(index.row());

        if(role == Qt::DisplayRole ) return item->toString();
        else if(role == PathRole) return item->path();
        else if(role == PartIvaRole) return item->partIva();
        else if(role == PartIvaFixedRole) return XmlList::fix(item->partIva());
        else if(role == IntestazioneRole) return item->intestazione();
        else if(role == IntestazioneFixedRole) return XmlList::fix(item->intestazione());
        else if(role == VDataRole) return item->vData();
        else if(role == VDataFixedRole) return XmlList::fix(item->vData());
        else if(role == DateRole) return item->date();
        else if(role == Imponibile_4Role) return item->imponibile_4();
        else if(role == Imposta_4Role) return item->imposta_4();
        else if(role == Imponibile_5Role) return item->imponibile_5();
        else if(role == Imposta_5Role) return item->imposta_5();
        else if(role == Imponibile_10Role) return item->imponibile_10();
        else if(role == Imposta_10Role) return item->imposta_10();
        else if(role == Imponibile_22Role) return item->imponibile_22();
        else if(role == Imposta_22Role) return item->imposta_22();
        else if(role == TotaleRole) return item->totale();
        else if(role == FileNameRole) {
            QFileInfo info(item->path());
            return info.fileName();
        }
        else if(role == FileNameFixedRole) {
            QFileInfo info(item->path());
            return XmlList::fix(info.fileName());
        }
        else if(role == SyncRole) return item->sync();
        else if(role == TipoRole) return item->tipo();
        else if(role == TipoStringaRole) return item->tipoStringa();
        else if(role == TipoStringaFixedRole) return XmlList::fix(item->tipoStringa());

        return {};
    }

    XmlFile* item(const QModelIndex & index) {
        XmlFile *item = nullptr;
        if (!hasIndex(index.row(), index.column(), index.parent()))
            return item;
        item = m_model.at(index.row());
        return item;
    }

private:
    QList<XmlFile*> m_model;

signals:
    void modelChanged();
    void rowInserted(const QModelIndex &parent);

private:
    void connectXmlFile(XmlFile *xmlFile){
        connect(xmlFile, &XmlFile::pathChanged, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::partIvaChanged, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::intestazioneChanged, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::vDataChanged, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::imponibile_4Changed, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::imposta_4Changed, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::imponibile_5Changed, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::imposta_5Changed, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::imponibile_10Changed, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::imposta_10Changed, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::imponibile_22Changed, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::imposta_22Changed, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::totaleChanged, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::syncChanged, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::tipoChanged, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::tipoStringaChanged, this, &XmlList::slotDataChanged);
        connect(xmlFile, &XmlFile::dateChanged, this, &XmlList::slotDataChanged);

    }
    void disconnectXmlFile(XmlFile *xmlFile){
        disconnect(xmlFile, &XmlFile::pathChanged, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::partIvaChanged, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::intestazioneChanged, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::vDataChanged, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::imponibile_4Changed, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::imposta_4Changed, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::imponibile_5Changed, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::imposta_5Changed, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::imponibile_10Changed, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::imposta_10Changed, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::imponibile_22Changed, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::imposta_22Changed, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::totaleChanged, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::syncChanged, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::tipoChanged, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::tipoStringaChanged, this, &XmlList::slotDataChanged);
        disconnect(xmlFile, &XmlFile::dateChanged, this, &XmlList::slotDataChanged);
    }

private slots:
    void slotDataChanged(){
        for(int i=0; i<this->rowCount(); i++){
            QModelIndex index = this->index(i,0);
            emit dataChanged(index, index);
        }
        emit modelChanged();
    }

};
Q_DECLARE_METATYPE(XmlList)
#endif //XML_LIST_H
