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

#ifndef _TelepathyQt4_account_model_h_HEADER_GUARD_
#define _TelepathyQt4_account_model_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_HEADER
#error IN_TELEPATHY_QT4_HEADER
#endif

#include <QAbstractListModel>

#include <TelepathyQt4/Account>
#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/Types>
#include <TelepathyQt4/TextChannel>

#include "account-model-item.h"

namespace Tp
{

class TELEPATHY_QT4_EXPORT AccountModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(int accountCount READ accountCount NOTIFY accountCountChanged)
    Q_ENUMS(Role)

public:
    enum Role {
        // general roles
        ItemRole = Qt::UserRole,
        AvatarRole,
        IdRole,

        // account roles
        ValidRole,
        EnabledRole,
        ConnectionManagerNameRole,
        ProtocolNameRole,
        DisplayNameRole,
        IconRole,
        NicknameRole,
        ConnectsAutomaticallyRole,
        ChangingPresenceRole,
        AutomaticPresenceRole,
        CurrentPresenceRole,
        CurrentPresenceTypeRole,
        CurrentPresenceStatusMessageRole,
        RequestedPresenceRole,
        RequestedPresenceTypeRole,
        RequestedPresenceStatusMessageRole,
        ConnectionStatusRole,
        ConnectionStatusReasonRole,

        // contact roles
        AliasRole,
        PresenceStatusRole,
        PresenceTypeRole,
        PresenceMessageRole,
        SubscriptionStateRole,
        PublishStateRole,
        BlockedRole,
        GroupsRole
    };

    explicit AccountModel(const Tp::AccountManagerPtr &am, QObject *parent = 0);
    virtual ~AccountModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    AccountPtr accountForIndex(const QModelIndex &index) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex index(TreeNode *node) const;
    virtual QModelIndex parent(const QModelIndex &index) const;

    int accountCount() const;
    Q_INVOKABLE QObject *accountItemForId(const QString &id) const;

Q_SIGNALS:
    void accountCountChanged();
    void accountConnectionStatusChanged(const QString &accountId,
                                        int status, int statusReason);

private:
    Tp::AccountManagerPtr mAM;
    class TreeNode *mTree;

    class TreeNode *node(const QModelIndex &index) const;

    int rowOf(const Account *account);
    ContactManager *contactManager(int row) const;

private Q_SLOTS:
    void onNewAccount(const Tp::AccountPtr &account);
    void onItemChanged(TreeNode *node);
    void onItemsAdded(TreeNode *parent, const QList<TreeNode *>& nodes);
    void onItemsRemoved(TreeNode *parent, int first, int last);
};

}

#endif // _TelepathyQt4_account_model_h_HEADER_GUARD_

