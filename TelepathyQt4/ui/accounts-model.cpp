#include "accounts-model.h"

#include <TelepathyQt4/PendingReady>

AccountsModel::AccountsModel(Tp::AccountManagerPtr am)
    : mAM(am)
    , mAccounts(am->allAccounts())
{
    connect(mAM->becomeReady(),
            SIGNAL(finished(Tp::PendingOperation *)),
            SLOT(onAMReady(Tp::PendingOperation *)));
}

void AccountsModel::onAMReady(Tp::PendingOperation *)
{
    QList<Tp::AccountPtr> accounts = mAM->allAccounts();
    beginInsertRows(QModelIndex(), 0, accounts.count() - 1);
    mAccounts = accounts;
    endInsertRows();
}

int AccountsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mAccounts.count();
}

int AccountsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant AccountsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= mAccounts.count()) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
        return mAccounts[index.row()]->serviceName();
    default:
        return QVariant();
    }
}

QVariant AccountsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        if (orientation == Qt::Horizontal)
            return QString("Column %1").arg(section);
        else
            return QVariant();
    default:
        return QVariant();
    }
}

