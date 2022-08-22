#ifndef ABSTRACTLISTMODEL_H
#define ABSTRACTLISTMODEL_H

#include <QAbstractListModel>
#include <RepoMacro>

class  AbstractListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ADD_PROPERTY(int, count, Count, 0)

public:
    explicit AbstractListModel(QObject *parent = nullptr);

private:
    void init();
    void initCount();

protected slots:
    void updateCount();
};

#endif // ABSTRACTLISTMODEL_H
