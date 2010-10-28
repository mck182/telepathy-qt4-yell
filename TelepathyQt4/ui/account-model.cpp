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
#include <TelepathyQt4/ContactManager>

#include "account-model-item.h"
#include "contact-model-item.h"

namespace Tp
{

AccountModel::AccountModel(const Tp::AccountManagerPtr &am, QObject *parent)
    : QAbstractItemModel(parent)
    , mAM(am)
{
    Q_ASSERT(mAM->isReady());
    mTree = new TreeNode;
    mAccounts = mAM->allAccounts();
    connect(mAM.data(),
            SIGNAL(newAccount(const Tp::AccountPtr &)),
            SLOT(onNewAccount(const Tp::AccountPtr &)));

    foreach (Tp::AccountPtr account, mAccounts) {
        setupAccount(account);
    }

    QHash<int, QByteArray> roles;
    roles[ItemRole] = "item";
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
    roles[IdRole] = "id";
    roles[AliasRole] = "aliasName";
    roles[AvatarRole] = "avatar";
    roles[PresenceStatusRole] = "presenceStatus";
    roles[PresenceTypeRole] = "presenceType";
    roles[PresenceMessageRole] = "presenceMessage";
    roles[SubscriptionStateRole] = "subscriptionState";
    roles[PublishStateRole] = "publishState";
    roles[BlockedRole] = "blocked";
    roles[GroupsRole] = "groups";
    setRoleNames(roles);
}

AccountModel::~AccountModel()
{
    delete mTree;
}

void AccountModel::setupAccount(const Tp::AccountPtr &account)
{
    TreeNode *accountNode = new AccountModelItem(account);
    if (account->haveConnection()) {
        ContactManager *manager = account->connection()->contactManager();
        foreach (ContactPtr contact, manager->allKnownContacts()) {
            accountNode->addChild(new ContactModelItem(contact));
        }
    }
    mTree->addChild(accountNode);

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

int AccountModel::rowOf(const Account *account)
{
    for (int i = 0; i < mAccounts.size(); i++) {
        if (mAccounts[i].data() == account) {
            return i;
        }
    }

    return -1;
}

ContactManager* AccountModel::contactManager(int row) const
{
    Tp::ConnectionPtr connection = mAccounts[row]->connection();
    if (connection) {
        return connection->contactManager();
    }
    else {
        return 0;
    }
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

int AccountModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

int AccountModel::rowCount(const QModelIndex &parent) const
{
    return node(parent)->size();
}

QVariant AccountModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    
    return node(index)->data(role);
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

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool AccountModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid()) {
        node(index)->setData(role, value);
    }

    return false;
}

QModelIndex AccountModel::index(int row, int column, const QModelIndex &parent) const
{
    TreeNode *parentNode = node(parent);
    return createIndex(row, column, parentNode->childAt(row));
}

QModelIndex AccountModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    TreeNode *currentNode = node(index);
    if (currentNode->parent()) {
        TreeNode *grandparent = currentNode->parent()->parent();
        if (grandparent) {
            return createIndex(grandparent->indexOf(currentNode->parent()), 0, currentNode->parent());
        }
    }

    // no parent or grandparent: return root node
    return QModelIndex();
}

TreeNode* AccountModel::node(const QModelIndex &index) const
{
    TreeNode *node = reinterpret_cast<TreeNode *>(index.internalPointer());
    return node ? node : mTree;
}

}
