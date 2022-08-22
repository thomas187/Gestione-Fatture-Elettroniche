#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <RepoMacro>

class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_ADD_PROPERTY(int, count, Count, 0)

public:
    explicit SortFilterProxyModel(QObject *parent = nullptr);

private:
    void init();
    void initCount();

protected slots:
    void updateCount();

public:
    void slotDataChanged(){
        for(int i=0; i<this->rowCount(); i++){
            QModelIndex index = this->index(i,0);
            emit dataChanged(index, index);
        }
    }

};

#endif // SORTFILTERPROXYMODEL_H
