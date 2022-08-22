#include "abstractlistmodel.h"

AbstractListModel::AbstractListModel(QObject *parent) : QAbstractListModel(parent)
{
    this->init();
}

void AbstractListModel::init()
{
    this->initCount();
}

void AbstractListModel::initCount()
{
    connect(this, &QAbstractListModel::rowsInserted, this, &AbstractListModel::updateCount);
    connect(this, &QAbstractListModel::rowsRemoved,  this, &AbstractListModel::updateCount);
    connect(this, &QAbstractListModel::modelReset,   this, &AbstractListModel::updateCount);
}

void AbstractListModel::updateCount()
{
    this->setCount(this->rowCount());
}
