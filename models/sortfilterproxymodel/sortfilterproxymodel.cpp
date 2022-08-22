#include "sortfilterproxymodel.h"

SortFilterProxyModel::SortFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    this->init();
}

void SortFilterProxyModel::init()
{
    this->initCount();
}

void SortFilterProxyModel::initCount()
{
    connect(this, &QSortFilterProxyModel::rowsInserted, this, &SortFilterProxyModel::updateCount);
    connect(this, &QSortFilterProxyModel::rowsRemoved,  this, &SortFilterProxyModel::updateCount);
    connect(this, &QSortFilterProxyModel::modelReset,   this, &SortFilterProxyModel::updateCount);
}

void SortFilterProxyModel::updateCount()
{
    this->setCount(this->rowCount());
}
