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

#include <TelepathyQt4/ui/ContactsListModel>
#include "TelepathyQt4/ui/_gen/contacts-list-model.moc.hpp"

#include <TelepathyQt4/Contact>
#include <TelepathyQt4/ContactManager>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/PendingContacts>

namespace Tp
{
    ContactsListModel::ContactsListModel(const Tp::AccountManagerPtr &am, QObject *parent)
     : QAbstractListModel(parent),
     mAM(am)
    {
        connect(mAM->becomeReady(),
                SIGNAL(finished(Tp::PendingOperation *)),
                SLOT(onAccountManagerReady(Tp::PendingOperation *)));

        QHash<int, QByteArray> roles;
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

    ContactsListModel::~ContactsListModel()
    {
    }

    void ContactsListModel::onAccountManagerReady(Tp::PendingOperation *)
    {
        Features features;
        features << Account::FeatureCore
                << Account::FeatureAvatar
                << Account::FeatureProtocolInfo
                << Account::FeatureCapabilities
                << Account::FeatureProfile;

        QList<AccountPtr> accountsList = mAM->allAccounts();
        QList<AccountPtr>::ConstIterator ac_it;
        for(ac_it = accountsList.constBegin(); ac_it != accountsList.constEnd(); ++ac_it) {
            AccountPtr accountPtr = (*ac_it);

            if(accountPtr->connectionStatus() == Tp::ConnectionStatusConnected) {
                connect(accountPtr->becomeReady(features),
                        SIGNAL(finished(Tp::PendingOperation *)),
                        SLOT(onAccountReady(Tp::PendingOperation *)));
            }
            
            connect(accountPtr.data(),
                    SIGNAL(statusChanged(Tp::ConnectionStatus,
                    Tp::ConnectionStatusReason, const QString &, const QVariantMap &)),
                    SLOT(onAccountStatusChanged(statusChanged(Tp::ConnectionStatus,
                    Tp::ConnectionStatusReason, const QString &, const QVariantMap &))));
        }
    }

    void ContactsListModel::onAccountStatusChanged(Tp::ConnectionStatus status,
            Tp::ConnectionStatusReason statusReason,
            const QString &error, const QVariantMap &errorDetails)
    {
        if(!error.isEmpty()) {
            qWarning("connection error: %s", qPrintable(error));
        }

        if(status == Tp::ConnectionStatusConnected) {
            Features features;
            features << Account::FeatureCore
                << Account::FeatureAvatar
                << Account::FeatureProtocolInfo
                << Account::FeatureCapabilities
                << Account::FeatureProfile;

            Tp::Account * account = qobject_cast<Tp::Account*>(sender());
            connect(account->becomeReady(features),
                        SIGNAL(finished(Tp::PendingOperation *)),
                        SLOT(onAccountReady(Tp::PendingOperation *)));
        }
    }

    void ContactsListModel::onAccountReady(Tp::PendingOperation* op)
    {
        if (op->isError()) {
            qWarning() << "Account cannot become ready";
            return;
        }

        PendingReady *pr = qobject_cast<PendingReady *>(op);
        AccountPtr accountPtr = AccountPtr(qobject_cast<Account *>(pr->object()));

        if(accountPtr->haveConnection()) {
           addConnection(accountPtr->connection());
        }
    }

    void ContactsListModel::onPresencePublicationRequested(const Tp::Contacts &contacts)
    {
        QList<Tp::ContactPtr> contactsList = contacts.toList();
        Tp::ContactManager* contactManager = qobject_cast<Tp::ContactManager*>(sender());
        upgradeContacts(contactManager, contactsList);
    }

    void ContactsListModel::addConnection(const ConnectionPtr &conn)
    {
        Features features;
        features << Connection::FeatureCore
                << Connection::FeatureSelfContact
                << Connection::FeatureSimplePresence
                << Connection::FeatureRoster
                << Connection::FeatureRosterGroups
                << Connection::FeatureAccountBalance;
        connect(conn->becomeReady(features),
                SIGNAL(finished(Tp::PendingOperation *)),
                SLOT(onConnectionReady(Tp::PendingOperation *)));
    }

    void ContactsListModel::removeConnection(const ConnectionPtr &conn)
    {
        mConns.removeOne(conn);
    }

    void ContactsListModel::onConnectionReady(Tp::PendingOperation *op)
    {
        if (op->isError()) {
            qWarning() << "Connection cannot become ready";
            return;
        }

        PendingReady *pr = qobject_cast<PendingReady *>(op);
        ConnectionPtr conn = ConnectionPtr(qobject_cast<Connection *>(pr->object()));
        mConns.append(conn);
        connect(conn.data(),
                SIGNAL(invalidated(Tp::DBusProxy *, const QString &, const QString &)),
                SLOT(onConnectionInvalidated(Tp::DBusProxy *, const QString &, const QString &)));
        connect(conn->contactManager(),
                SIGNAL(presencePublicationRequested(const Tp::Contacts &)),
                SLOT(onPresencePublicationRequested(const Tp::Contacts &)));

        QList<Tp::ContactPtr> contacts = conn->contactManager()->allKnownContacts().toList();
        upgradeContacts(conn->contactManager(), contacts);
    }
    
    void ContactsListModel::upgradeContacts(ContactManager* contactManager, QList<Tp::ContactPtr> &contacts)
    {
        //upgrade features for contacts
        QSet<Contact::Feature> features;
        features << Contact::FeatureAlias
                << Contact::FeatureAvatarToken
                << Contact::FeatureSimplePresence
                << Contact::FeatureCapabilities
                << Contact::FeatureLocation
                << Contact::FeatureInfo
                << Contact::FeatureAvatarData;

        connect(contactManager->upgradeContacts(contacts, features),
                SIGNAL(finished(Tp::PendingOperation *)),
                SLOT(onContactsUpgraded(Tp::PendingOperation *)));
    }

    void ContactsListModel::onContactsUpgraded(Tp::PendingOperation *op)
    {
        if (op->isError()) {
            qWarning() << "Contacts cannot be upgraded";
            return;
        }

        Tp::PendingContacts *pendingContacts = qobject_cast<PendingContacts *>(op);
        QList<Tp::ContactPtr> contacts = pendingContacts->contacts();

        //add items to model
        beginInsertRows(QModelIndex(), 0, contacts.count() - 1);
        mContacts.append(contacts);
        endInsertRows();
        emit contactCountChanged();
    }

    void ContactsListModel::onConnectionInvalidated(DBusProxy *proxy,
            const QString &errorName, const QString &errorMessage)
    {
        qWarning() << "onConnectionInvalidated: connection became invalid:" <<
            errorName << "-" << errorMessage;
        foreach (const ConnectionPtr &conn, mConns) {
            if (conn.data() == proxy) {
                mConns.removeOne(conn);
            }
        }
        emit contactCountChanged();
    }

    int ContactsListModel::rowCount(const QModelIndex &) const
    {
        return mContacts.count();
    }

    QVariant ContactsListModel::data(const QModelIndex &index, int role) const
    {
        // Only column 0 is valid.
        if (index.column() != 0) {
            return QVariant();
        }

        if (index.row() >= mContacts.count())
            return QVariant();

        Tp::ContactPtr contactItem = mContacts[index.row()];
        QVariant data;

        switch(role)
        {
            case Qt::DisplayRole:
                data.setValue<QString>(contactItem->alias());
                break;
            case ContactsListModel::IdRole:
                data.setValue<QString>(contactItem->id());
                break;
            case ContactsListModel::AliasRole:
                data.setValue<QString>(contactItem->alias());
                break;
            case ContactsListModel::PresenceStatusRole:
                data.setValue<QString>(contactItem->presenceStatus());
                break;
            case ContactsListModel::PresenceTypeRole:
                data.setValue<qint64>(contactItem->presenceType());
                break;
            case ContactsListModel::PresenceMessageRole:
                data.setValue<QString>(contactItem->presenceMessage());
                break;
            case ContactsListModel::SubscriptionStateRole:
                data.setValue<qint64>(contactItem->subscriptionState());
                break;
            case ContactsListModel::PublishStateRole:
                data.setValue<qint64>(contactItem->publishState());
                break;
            case ContactsListModel::BlockedRole:
                data.setValue<bool>(contactItem->block());
                break;
            case ContactsListModel::GroupsRole:
                data.setValue<QStringList>(contactItem->groups());
                break;
            case ContactsListModel::AvatarRole:
                data.setValue<QString>(contactItem->avatarData().fileName);
                break;
            default:
                break;
        }
        return data;
    }
    
    ContactPtr ContactsListModel::contactForIndex(const QModelIndex &index) const
    {
        return mContacts.at(index.row());
    }
}

