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

#include "TelepathyQt4/ui/account-model-item.h"
#include "TelepathyQt4/ui/contact-model-item.h"

namespace Tp
{

AccountModel::AccountModel(const Tp::AccountManagerPtr &am, QObject *parent)
    : QAbstractItemModel(parent),
      mAM(am)
{
    mTree = new TreeNode;
    connect(mTree,
            SIGNAL(changed(TreeNode *)),
            SLOT(onItemChanged(TreeNode *)));
    connect(mTree,
            SIGNAL(childrenAdded(TreeNode *, QList<TreeNode *>)),
            SLOT(onItemsAdded(TreeNode *, QList<TreeNode *>)));
    connect(mTree,
            SIGNAL(childrenRemoved(TreeNode *, int, int)),
            SLOT(onItemsRemoved(TreeNode *, int, int)));

    foreach (Tp::AccountPtr account, mAM->allAccounts()) {
        AccountModelItem *item = new AccountModelItem(account);
        connect(item, SIGNAL(connectionStatusChanged(const QString&, int, int)),
                this, SIGNAL(accountConnectionStatusChanged(const QString&, int, int)));
        mTree->addChild(item);
    }

    connect(mAM.data(),
            SIGNAL(newAccount(const Tp::AccountPtr &)),
            SLOT(onNewAccount(const Tp::AccountPtr &)));

    QHash<int, QByteArray> roles;
    roles[ItemRole] = "item";
    roles[IdRole] = "id";
    roles[ValidRole] = "valid";
    roles[EnabledRole] = "enabled";
    roles[ConnectionManagerNameRole] = "connectionManager";
    roles[ProtocolNameRole] = "protocol";
    roles[DisplayNameRole] = "displayName";
    roles[IconRole] = "icon";
    roles[NicknameRole] = "nickname";
    roles[ConnectsAutomaticallyRole] = "connectsAutomatically";
    roles[ChangingPresenceRole] = "changingPresence";
    roles[AutomaticPresenceRole] = "automaticPresence";
    roles[CurrentPresenceRole] = "status";
    roles[CurrentPresenceTypeRole] = "statusType";
    roles[CurrentPresenceStatusMessageRole] = "statusMessage";
    roles[RequestedPresenceRole] = "requestedStatus";
    roles[RequestedPresenceTypeRole] = "requestedStatusType";
    roles[RequestedPresenceStatusMessageRole] = "requestedStausMessage";
    roles[ConnectionStatusRole] = "connectionStatus";
    roles[ConnectionStatusReasonRole] = "connectionStatusReason";
    roles[ConnectionRole] = "connection";
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

void AccountModel::onNewAccount(const Tp::AccountPtr &account)
{
    AccountModelItem *accountNode = new AccountModelItem(account);

    connect(accountNode, SIGNAL(connectionStatusChanged(const QString&, int, int)),
            this, SIGNAL(accountConnectionStatusChanged(const QString&, int, int)));

    onItemsAdded(mTree, QList<TreeNode *>() << accountNode);
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
    qDebug() << "adding rows from" << currentSize << "to" << (currentSize + nodes.size());
    beginInsertRows(parentIndex, currentSize, currentSize + nodes.size() - 1);
    foreach (TreeNode *node, nodes) {
        parent->addChild(node);
    }
    endInsertRows();
    emit accountCountChanged();
}

void AccountModel::onItemsRemoved(TreeNode *parent, int first, int last)
{
    QModelIndex parentIndex = index(parent);
    QList<TreeNode *> removedItems;
    beginRemoveRows(parentIndex, first, last);
    for (int i = last; i >= first; i--) {
        parent->childAt(i)->remove();
    }
    endRemoveRows();
    emit accountCountChanged();
}

int AccountModel::accountCount() const
{
    return mTree->size();
}

QObject *AccountModel::accountItemForId(const QString &id) const
{
    for (int i = 0; i < mTree->size(); ++i) {
        AccountModelItem *item = qobject_cast<AccountModelItem*>(mTree->childAt(i));
        if (!item) {
            continue;
        }

        if (item->data(IdRole) == id) {
            return item;
        }
    }

    return 0;
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
    TreeNode *accountNode = node(index);
    AccountModelItem *item = qobject_cast<AccountModelItem *>(accountNode);
    if (item) {
        return item->account();
    }
    else {
        return AccountPtr();
    }
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
