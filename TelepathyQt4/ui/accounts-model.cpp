#include "accounts-model.h"

#include <TelepathyQt4/PendingReady>

AccountsModel::AccountsModel(Tp::AccountManagerPtr am)
    : mAM(am)
    , mAccounts(am->allAccounts())
{
    connect(mAM->becomeReady(),
            SIGNAL(finished(Tp::PendingOperation *)),
            SLOT(onAMReady(Tp::PendingOperation *)));
    QHash<int, QByteArray> roles;
    roles[ValidRole] = "valid";
    roles[EnabledRole] = "enabled";
    roles[ConnectionManagerRole] = "connection";
    roles[ProtocolNameRole] = "protocol";
    roles[DisplayNameRole] = "displayName";
    roles[NicknameRole] = "nickname";
    roles[ConnectsAutomaticallyRole] = "connectsAutomatically";
    roles[ChangingPresenceRole] = "changingPresence";
    roles[AutomaticPresenceRole] = "automaticPresence";
    roles[CurrentPresenceRole] = "currentPresence";
    roles[RequestedPresenceRole] = "requestedPresence";
    roles[ConnectionStatusRole] = "connectionStatus";
    roles[ConnectionRole] = "connection";
    setRoleNames(roles);
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

QVariant AccountsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= mAccounts.count()) {
        return QVariant();
    }

    Tp::AccountPtr account = mAccounts[index.row()];
    switch (role) {
        case ValidRole:
            return account->isValid();
        case EnabledRole:
            return account->isEnabled();
        case ConnectionManagerRole:
            return account->cmName();
        case ProtocolNameRole:
            return account->protocolName();
        case DisplayNameRole:
        case Qt::DisplayRole:
            return account->displayName();
        case NicknameRole:
            return account->nickname();
        case ConnectsAutomaticallyRole:
            return account->connectsAutomatically();
        case ChangingPresenceRole:
            return account->isChangingPresence();
        case AutomaticPresenceRole:
            return account->automaticPresence().status;
        case CurrentPresenceRole:
            return account->currentPresence().status;
        case RequestedPresenceRole:
            return account->requestedPresence().status;
        case ConnectionStatusRole:
            return account->connectionStatus();
        case ConnectionRole:
            return account->connectionObjectPath();
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

