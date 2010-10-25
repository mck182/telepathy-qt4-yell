/*
 * This file is part of TelepathyQt4
 *
 * Copyright (C) 2008-2010 Collabora Ltd. <http://www.collabora.co.uk/>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <TelepathyQt4/ui/AccountModel>
#include "TelepathyQt4/ui/_gen/account-model.moc.hpp"
#include <TelepathyQt4/PendingReady>

namespace Tp
{

AccountModel::AccountModel(const Tp::AccountManagerPtr &am, QObject *parent)
    : QAbstractListModel(parent)
    , mAM(am)
{
    connect(mAM->becomeReady(),
            SIGNAL(finished(Tp::PendingOperation *)),
            SLOT(onAMReady(Tp::PendingOperation *)));
    QHash<int, QByteArray> roles;
    roles[ValidRole] = "valid";
    roles[EnabledRole] = "enabled";
    roles[ConnectionManagerRole] = "connectionManager";
    roles[ProtocolNameRole] = "protocol";
    roles[DisplayNameRole] = "displayName";
    roles[NicknameRole] = "nickname";
    roles[ConnectsAutomaticallyRole] = "connectsAutomatically";
    roles[ChangingPresenceRole] = "changingPresence";
    roles[AutomaticPresenceRole] = "automaticPresence";
    roles[CurrentPresenceRole] = "status";
    roles[CurrentStatusMessage] = "statusMessage";
    roles[RequestedPresenceRole] = "requestedStatus";
    roles[RequestedStatusMessage] = "requestedStausMessage";
    roles[ConnectionStatusRole] = "connectionStatus";
    roles[ConnectionRole] = "connection";
    setRoleNames(roles);
}

AccountModel::~AccountModel()
{
}

void AccountModel::onAMReady(Tp::PendingOperation *)
{
    QList<Tp::AccountPtr> accounts = mAM->allAccounts();
    beginInsertRows(QModelIndex(), 0, accounts.count() - 1);
    mAccounts = accounts;
    endInsertRows();

    emit accountCountChanged();

    connect(mAM.data(),
            SIGNAL(newAccount(const Tp::AccountPtr &)),
            SLOT(onNewAccount(const Tp::AccountPtr &)));

    foreach (Tp::AccountPtr account, mAccounts) {
        setupAccount(account);
    }
}

void AccountModel::setupAccount(const Tp::AccountPtr &account)
{
    connect(account.data(),
            SIGNAL(removed()),
            SLOT(onAccountRemoved()));
    connect(account.data(),
            SIGNAL(serviceNameChanged(QString)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(profileChanged(const Tp::ProfilePtr &)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(iconNameChanged(QString)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(nicknameChanged(QString)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(normalizedNameChanged(QString)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(validityChanged(bool)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(stateChanged(bool)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(capabilitiesChanged(Tp::ConnectionCapabilities *)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(connectsAutomaticallyPropertyChanged(bool)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(parametersChanged(QVariantMap)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(changingPresence(bool)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(automaticPresenceChanged(Tp::SimplePresence)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(currentPresenceChanged(Tp::SimplePresence)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(requestedPresenceChanged(Tp::SimplePresence)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(onlinenessChanged(bool)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(avatarChanged(Tp::Avatar)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(statusChanged(Tp::ConnectionStatus, Tp::ConnectionStatusReason,
                                 QString, QVariantMap)),
            SLOT(onAccountChanged()));
    connect(account.data(),
            SIGNAL(haveConnectionChanged(bool)),
            SLOT(onAccountChanged()));
}

void AccountModel::onNewAccount(const Tp::AccountPtr &account)
{
    beginInsertRows(QModelIndex(), mAccounts.count(), mAccounts.count());
    mAccounts.append(account);
    endInsertRows();

    setupAccount(account);
    emit accountCountChanged();
}

void AccountModel::onAccountRemoved()
{
    Account *account = qobject_cast<Account *>(sender());
    Q_ASSERT(account);

    for (int i = 0; i < mAccounts.size(); i++) {
        if (mAccounts[i].data() == account) {
            beginRemoveRows(QModelIndex(), i, i);
            mAccounts.removeAt(i);
            endRemoveRows();
        }
    }

    emit accountCountChanged();
}

void AccountModel::onAccountChanged()
{
    Account *account = qobject_cast<Account *>(sender());
    Q_ASSERT(account);

    for (int i = 0; i < mAccounts.size(); i++) {
        if (mAccounts[i].data() == account) {
            emit dataChanged(index(i), index(i));
            return;
        }
    }

    qWarning() << "Received change notification from unknown account";
}

int AccountModel::accountCount() const
{
    return mAccounts.count();
}

int AccountModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mAccounts.count();
}

QVariant AccountModel::data(const QModelIndex &index, int role) const
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
        case CurrentStatusMessage:
            qDebug() << "Returning the current status message, which is:" << account->currentPresence().statusMessage;
            return account->currentPresence().statusMessage;
        case RequestedPresenceRole:
            return account->requestedPresence().status;
        case RequestedStatusMessage:
            return account->requestedPresence().statusMessage;
        case ConnectionStatusRole:
            return account->connectionStatus();
        case ConnectionRole:
            return account->connectionObjectPath();
        default:
            return QVariant();
    }
}

AccountPtr AccountModel::accountForIndex(const QModelIndex &index) const
{
    return mAccounts.at(index.row());
}

Qt::ItemFlags AccountModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

bool AccountModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid()) {
        switch (role) {
        case EnabledRole:
            setAccountEnabled(index.row(), value.toBool());
            break;
        case RequestedPresenceRole:
            setAccountStatus(index.row(), value.toString());
            break;
        case RequestedStatusMessage:
            setAccountStatusMessage(index.row(), value.toString());
            break;
        case NicknameRole:
            setAccountNickname(index.row(), value.toString());
            break;
        default:
            return false;
        }

        return true;
    }

    return false;
}

void AccountModel::setAccountEnabled(int row, bool value)
{
    mAccounts[row]->setEnabled(value);
    emit dataChanged(index(row), index(row));
}

void AccountModel::setAccountStatus(int row, const QString &value)
{
    AccountPtr account = mAccounts[row];
    SimplePresence presence = account->currentPresence();
    presence.status = value;
    account->setRequestedPresence(presence);
    emit dataChanged(index(row), index(row));
}

void AccountModel::setAccountStatusMessage(int row, const QString& value)
{
    AccountPtr account = mAccounts[row];
    SimplePresence presence = account->currentPresence();
    presence.statusMessage = value;
    account->setRequestedPresence(presence);
    emit dataChanged(index(row), index(row));
}

void AccountModel::setAccountNickname(int row, const QString &value)
{
    mAccounts[row]->setNickname(value);
    emit dataChanged(index(row), index(row));
}

}
