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

namespace Tp
{



#include "contacts-list-model.moc"

    ContactsListModel::ContactsListModel(QObject *parent)
     : QAbstractItemModel(parent)
    {


    }



    void ContactsListModel::onConnectionReady(Tp::PendingOperation *op)
    {
        if (op->isError()) {
            qWarning() << "Connection cannot become ready";
            return;
        }

        PendingReady *pr = qobject_cast<PendingReady *>(op);
        ConnectionPtr conn = ConnectionPtr(qobject_cast<Connection *>(pr->object()));
        connect(conn->contactManager(),
                SIGNAL(presencePublicationRequested(const Tp::Contacts &)),
                SLOT(onPresencePublicationRequested(const Tp::Contacts &)));

        qDebug() << "Connection ready";
        RosterItem *item;
        bool exists;
        foreach (const ContactPtr &contact, conn->contactManager()->allKnownContacts()) {
            exists = false;
            item = createItemForContact(contact, exists);
            if (!exists) {
                connect(item, SIGNAL(changed()), SLOT(updateActions()));
            }
        }

        mAddBtn->setEnabled(true);
    }

    QModelIndex ContactsListModel::index(int row, int column, const QModelIndex &parent) const
    {
        // 1 column list, so invalid index if the column is not 1.
        if (parent.isValid() && parent.column() != 0) {
            return QModelIndex();
        }

        // Get the parent item.
        AbstractTreeItem *parentItem = item(parent);

        // Get all the parent's children.
        QList<AbstractTreeItem*> children = parentItem->childItems();

        // Check the row doesn't go beyond the end of the list of children.
        if (row >= children.length()) {
            return QModelIndex();
        }

        // Return the index to the item.
        return createIndex(row, column, children.at(row));
    }

    QModelIndex ContactsListModel::parent(const QModelIndex &index) const
    {
        // If the index is invalid, return an invalid parent index.
        if (!index.isValid()) {
            return QModelIndex();
        }

        // Get the item we have been passed, and it's parent
        AbstractTreeItem *childItem = item(index);
        AbstractTreeItem *parentItem = childItem->parentItem();

        // If the parent is the root item, then the parent index of the index we were passed is
        // by definition an invalid index.
        if (parentItem == m_rootItem) {
            return QModelIndex();
        }

        // The parent of the item is not the root item, meaning that the parent must have a parent too.
        AbstractTreeItem *parentOfParentItem = parentItem->parentItem();

        // As stated in the previous comment, something is really wrong if it doesn't have a parent.
        Q_ASSERT(parentOfParentItem);
        if (!parentOfParentItem) {
            kWarning() << "Impossible parent situation occurred!";
            return createIndex(0, 0, parentItem);
        }

        // Return the model index of the parent item.
        return createIndex(parentOfParentItem->childItems().lastIndexOf(parentItem), 0, parentItem);
    }

    int ContactsListModel::rowCount(const QModelIndex &parent) const
    {
        // If the parent is invalid, then this request is for the root item.
        if (!parent.isValid()) {
            return m_rootItem->childItems().length();
        }

        // Get the item from the internal pointer of the ModelIndex.
        AbstractTreeItem *item = static_cast<AbstractTreeItem*>(parent.internalPointer());

        // If the item is valid, return the number of children it has.
        if (item) {
            return item->childItems().length();
        }

        // Otherwise, return 0
        return 0;
    }

    int ContactsListModel::columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);

        // All items have the same number of columns
        return 1;
    }

    QVariant ContactsListModel::data(const QModelIndex &index, int role) const
    {
        // Only column 0 is valid.
        if (index.column() != 0) {
            return QVariant();
        }

        // Check what type of item we have here.
        AbstractTreeItem *abstractItem = static_cast<AbstractTreeItem*>(index.internalPointer());
        ContactItem *contactItem = dynamic_cast<ContactItem*>(abstractItem);

        if (contactItem) {

            QVariant data;

            switch(role)
            {
            case Qt::DisplayRole:
                data.setValue<QString>(contactItem->accountIdentifier());
                break;
            case Qt::DecorationRole:
                data.setValue<QIcon>(contactItem->presenceIcon());
                break;
            case ContactsListModel::PresenceTypeRole:
                data.setValue<qint64>(contactItem->presenceType());
                break;
            case ContactsListModel::GroupsRole:
                data.setValue<QStringList>(contactItem->groups());
                break;
            case ContactsListModel::AvatarRole:
                data.setValue<QPixmap>(contactItem->avatar());
                break;
            case ContactsListModel::PersonContactResourceRole:
                data.setValue<QUrl>(contactItem->personContact().resourceUri());
                break;
            default:
                break;
            }

            return data;
        }
    }
