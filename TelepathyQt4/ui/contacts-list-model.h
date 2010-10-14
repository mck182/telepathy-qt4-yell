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

#include <QtCore/QAbstractItemModel>

#include <TelepathyQt4/Contact>
#include <abstract-tree-item.h>

namespace Tp
{

class ContactsListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum {
        IdRole = Qt::UserRole,
        ContactRole,
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

    explicit ContactsListModel(QObject *parent = 0);
    virtual ~ContactsListModel();

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QModelIndex index(int row,
                              int column,
                              const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

private Q_SLOTS:
    void onConnectionReady(Tp::PendingOperation *);
    void onPresencePublicationRequested(const Tp::Contacts &);


private:
    AbstractTreeItem* createContactItem(const Tp::ContactPtr &contact, bool &exists);
    AbstractTreeItem *item(const QModelIndex &index) const;

    QList<Tp::ConnectionPtr> mConns;

    AbstractTreeItem *m_rootItem;
};

}

#endif // _TelepathyQt4_contacts_list_model_h_HEADER_GUARD_
