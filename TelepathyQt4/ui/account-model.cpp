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
    mTree = new TreeNode;
    connect(mTree,
            SIGNAL(changed(TreeNode *)),
            SLOT(onItemChanged(TreeNode *)));
    connect(mTree,
            SIGNAL(removed(TreeNode *)),
            SLOT(onItemRemoved(TreeNode *)));

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
    mTree->addChild(accountNode);
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

void AccountModel::onItemRemoved(TreeNode *node)
{
    QModelIndex accountIndex = index(node);
    beginRemoveRows(accountIndex.parent(), accountIndex.row(), accountIndex.row());
    node->remove();
    endRemoveRows();

    emit accountCountChanged();
}

void AccountModel::onItemChanged(TreeNode *node)
{
    QModelIndex accountIndex = index(node);
    emit dataChanged(accountIndex, accountIndex);
}

void AccountModel::onItemsAdded(TreeNode *parent, const QList<TreeNode *>& nodes)
{
    QModelIndex parentIndex = index(parent);
    int currentSize = rowCount(parentIndex);
    beginInsertRows(parentIndex, currentSize, currentSize + nodes.size() - 1);
    foreach (TreeNode *node, nodes) {
        parent->addChild(node);
    }
    endInsertRows();
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

QModelIndex AccountModel::index(TreeNode *node) const
{
    if (node->parent()) {
        return createIndex(node->parent()->indexOf(node), 0, node);
    }
    else {
        return QModelIndex();
    }
}

QModelIndex AccountModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    TreeNode *currentNode = node(index);
    if (currentNode->parent()) {
        return AccountModel::index(currentNode->parent());
    }
    else {
        // no parent: return root node
        return QModelIndex();
    }
}

TreeNode* AccountModel::node(const QModelIndex &index) const
{
    TreeNode *node = reinterpret_cast<TreeNode *>(index.internalPointer());
    return node ? node : mTree;
}

}
