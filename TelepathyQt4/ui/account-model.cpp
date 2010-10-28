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

namespace Tp
{

class TreeNode {

public:

    TreeNode()
        : mParent(0)
    { }

    virtual ~TreeNode()
    {
        foreach (TreeNode *child, mChildren) {
            delete child;
        }
    }

    TreeNode *childAt(int index) const
    {
        return mChildren[index];
    }

    void addChild(TreeNode *node)
    {
        // takes ownership of node
        mChildren.append(node);
        node->mParent = this;
    }

    int indexOf(TreeNode *node) const {
        return mChildren.indexOf(node);
    }

    int size() const {
        return mChildren.size();
    }

    TreeNode *parent() const { return mParent; }

    virtual QVariant data(int role) const { return QVariant(); }
    virtual bool setData(int role, const QVariant &value) { return false; }

private:
    
    QList<TreeNode *> mChildren;
    TreeNode *mParent;
};

class AccountNode : public TreeNode {

public:

    AccountNode(const AccountPtr &account)
        : mAccount(account)
    { }

    virtual QVariant data(int role) const
    {
        switch (role) {
            case AccountModel::ValidRole:
                return mAccount->isValid();
            case AccountModel::EnabledRole:
                return mAccount->isEnabled();
            case AccountModel::ConnectionManagerRole:
                return mAccount->cmName();
            case AccountModel::ProtocolNameRole:
                return mAccount->protocolName();
            case AccountModel::DisplayNameRole:
            case Qt::DisplayRole:
                return mAccount->displayName();
            case AccountModel::NicknameRole:
                return mAccount->nickname();
            case AccountModel::ConnectsAutomaticallyRole:
                return mAccount->connectsAutomatically();
            case AccountModel::ChangingPresenceRole:
                return mAccount->isChangingPresence();
            case AccountModel::AutomaticPresenceRole:
                return mAccount->automaticPresence().status;
            case AccountModel::CurrentPresenceRole:
                return mAccount->currentPresence().status;
            case AccountModel::CurrentStatusMessage:
                return mAccount->currentPresence().statusMessage;
            case AccountModel::RequestedPresenceRole:
                return mAccount->requestedPresence().status;
            case AccountModel::RequestedStatusMessage:
                return mAccount->requestedPresence().statusMessage;
            case AccountModel::ConnectionStatusRole:
                return mAccount->connectionStatus();
            case AccountModel::ConnectionRole:
                return mAccount->connectionObjectPath();
            default:
                return QVariant();
        }
    }

    virtual bool setData(int role, const QVariant &value)
    {
        switch (role) {
        case AccountModel::EnabledRole:
            setEnabled(value.toBool());
            return true;
        case AccountModel::RequestedPresenceRole:
            setStatus(value.toString());
            return true;
        case AccountModel::RequestedStatusMessage:
            setStatusMessage(value.toString());
            return true;
        case AccountModel::NicknameRole:
            setNickname(value.toString());
            return true;
        default:
            return false;
        }
    }

    void setEnabled(bool value)
    {
        mAccount->setEnabled(value);
    }

    void setStatus(const QString &value)
    {
        SimplePresence presence = mAccount->currentPresence();
        presence.status = value;
        mAccount->setRequestedPresence(presence);
    }

    void setStatusMessage(const QString& value)
    {
        SimplePresence presence = mAccount->currentPresence();
        presence.statusMessage = value;
        mAccount->setRequestedPresence(presence);
    }

    void setNickname(const QString &value)
    {
        mAccount->setNickname(value);
    }

    void setPresence(int type, const QString &status, const QString &statusMessage)
    {
        SimplePresence presence;
        presence.type = type;
        presence.status = status;
        presence.statusMessage = statusMessage;
        mAccount->setRequestedPresence(presence);
    }

private:

    AccountPtr mAccount;
};

class ContactNode : public TreeNode
{
public:
    
    ContactNode(const Tp::ContactPtr &contact)
        : mContact(contact)
    { }

    virtual QVariant data(int role) const
    {
        switch(role)
        {
            case AccountModel::IdRole:
                return mContact->id();
            case Qt::DisplayRole:
            case AccountModel::AliasRole:
                return mContact->alias();
            case AccountModel::PresenceStatusRole:
                return mContact->presenceStatus();
            case AccountModel::PresenceTypeRole:
                return mContact->presenceType();
            case AccountModel::PresenceMessageRole:
                return mContact->presenceMessage();
            case AccountModel::SubscriptionStateRole:
                return mContact->subscriptionState();
            case AccountModel::PublishStateRole:
                return mContact->publishState();
            case AccountModel::BlockedRole:
                return !!mContact->block();
            case AccountModel::GroupsRole:
                return mContact->groups();
            case AccountModel::AvatarRole:
                return mContact->avatarData().fileName;
            default:
                break;
        }

        return QVariant();
    }

private:
    
    ContactPtr mContact;
};

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
    TreeNode *accountNode = new AccountNode(account);
    if (account->haveConnection()) {
        ContactManager *manager = account->connection()->contactManager();
        foreach (ContactPtr contact, manager->allKnownContacts()) {
            accountNode->addChild(new ContactNode(contact));
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
