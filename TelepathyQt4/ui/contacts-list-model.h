/*
 * This file is part of TelepathyQt4
 *
 * Copyright (C) 2008-2010 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2008-2010 Nokia Corporation
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

#ifndef _TelepathyQt4_contacts_list_model_h_HEADER_GUARD_
#define _TelepathyQt4_contacts_list_model_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_HEADER
#error IN_TELEPATHY_QT4_HEADER
#endif

#include <QAbstractListModel>

#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/Contact>
#include <TelepathyQt4/Connection>
#include <TelepathyQt4/Types>

namespace Tp
{

class TELEPATHY_QT4_EXPORT ContactsListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum {
        IdRole = Qt::UserRole,
        AliasRole,
        AvatarRole,
        PresenceStatusRole,
        PresenceTypeRole,
        PresenceMessageRole,
        SubscriptionStateRole,
        PublishStateRole,
        BlockedRole,
        GroupsRole
    };

    explicit ContactsListModel(const Tp::AccountManagerPtr &am, QObject *parent = 0);
    virtual ~ContactsListModel();

    QVariant data(const QModelIndex &index, int role) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    void addConnection(const Tp::ConnectionPtr &conn);
    void removeConnection(const Tp::ConnectionPtr &conn);
    
    ContactPtr contactForIndex(const QModelIndex &index) const;

Q_SIGNALS:
    void contactCountChanged(void);
    
public Q_SLOTS:
    void onAccountStatusChanged(Tp::ConnectionStatus status,
            Tp::ConnectionStatusReason statusReason,
            const QString &error, const QVariantMap &errorDetails);
    
protected:
    void upgradeContacts(ContactManager* contactManager, QList<Tp::ContactPtr> &contacts);

private Q_SLOTS:
    void onAccountManagerReady(Tp::PendingOperation *);
    void onAccountReady(Tp::PendingOperation* op);
    void onConnectionReady(Tp::PendingOperation *);
    void onContactsUpgraded(Tp::PendingOperation *op);
    void onPresencePublicationRequested(const Tp::Contacts &contacts);
    void onConnectionInvalidated(Tp::DBusProxy *,
            const QString &, const QString &);

private:

    Tp::AccountManagerPtr mAM;
    QList<Tp::ConnectionPtr> mConns;
    QList<Tp::ContactPtr> mContacts;

    //AbstractTreeItem *mRootItem;
};

}

#endif // _TelepathyQt4_contacts_list_model_h_HEADER_GUARD_
