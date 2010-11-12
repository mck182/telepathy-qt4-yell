/*
 * This file is part of TelepathyQt4
 *
 * Copyright (C) 2010 Collabora Ltd. <http://www.collabora.co.uk/>
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

#include "TelepathyQt4/models/accounts-model-item.h"

#include "TelepathyQt4/models/_gen/accounts-model-item.moc.hpp"

#include <TelepathyQt4/Account>
#include <TelepathyQt4/ContactManager>

#include <TelepathyQt4/models/AccountsModel>
#include <TelepathyQt4/models/AvatarImageProvider>

#include "TelepathyQt4/models/contact-model-item.h"

namespace Tp
{

AccountsModelItem::AccountsModelItem(const AccountPtr &account)
    : mAccount(account)
{
    if (mAccount->haveConnection()) {
        ContactManager *manager = account->connection()->contactManager();
        foreach (ContactPtr contact, manager->allKnownContacts()) {
            addChild(new ContactModelItem(contact));
        }

        connect(manager,
                SIGNAL(allKnownContactsChanged(Tp::Contacts, Tp::Contacts,
                                               Tp::Channel::GroupMemberChangeDetails)),
                SLOT(onContactsChanged(Tp::Contacts, Tp::Contacts)));
    }

    connect(mAccount.data(),
            SIGNAL(removed()),
            SLOT(onRemoved()));
    connect(mAccount.data(),
            SIGNAL(serviceNameChanged(QString)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(profileChanged(const Tp::ProfilePtr &)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(iconNameChanged(QString)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(nicknameChanged(QString)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(normalizedNameChanged(QString)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(validityChanged(bool)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(stateChanged(bool)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(capabilitiesChanged(Tp::ConnectionCapabilities *)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(connectsAutomaticallyPropertyChanged(bool)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(parametersChanged(QVariantMap)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(changingPresence(bool)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(automaticPresenceChanged(Tp::SimplePresence)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(currentPresenceChanged(Tp::SimplePresence)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(requestedPresenceChanged(Tp::SimplePresence)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(onlinenessChanged(bool)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(avatarChanged(Tp::Avatar)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(statusChanged(Tp::ConnectionStatus, Tp::ConnectionStatusReason,
                                 QString, QVariantMap)),
            SLOT(onChanged()));
    connect(mAccount.data(),
            SIGNAL(statusChanged(Tp::ConnectionStatus,Tp::ConnectionStatusReason,QString,QVariantMap)),
            SLOT(onStatusChanged(Tp::ConnectionStatus,Tp::ConnectionStatusReason,QString,QVariantMap)));
    connect(mAccount.data(),
            SIGNAL(haveConnectionChanged(bool)),
            SLOT(onChanged()));
}

QVariant AccountsModelItem::data(int role) const
{
    switch (role) {
        case AccountsModel::ItemRole:
            return QVariant::fromValue(
                const_cast<QObject *>(
                    static_cast<const QObject *>(this)));
        case AccountsModel::IdRole:
            return mAccount->uniqueIdentifier();
        case AccountsModel::AvatarRole:
            return AvatarImageProvider::urlFor(mAccount);
        case AccountsModel::ValidRole:
            return mAccount->isValid();
        case AccountsModel::EnabledRole:
            return mAccount->isEnabled();
        case AccountsModel::ConnectionManagerNameRole:
            return mAccount->cmName();
        case AccountsModel::ProtocolNameRole:
            return mAccount->protocolName();
        case AccountsModel::DisplayNameRole:
        case Qt::DisplayRole:
            return mAccount->displayName();
        case AccountsModel::IconRole:
            return mAccount->icon();
        case AccountsModel::NicknameRole:
            return mAccount->nickname();
        case AccountsModel::ConnectsAutomaticallyRole:
            return mAccount->connectsAutomatically();
        case AccountsModel::ChangingPresenceRole:
            return mAccount->isChangingPresence();
        case AccountsModel::AutomaticPresenceRole:
            return mAccount->automaticPresence().status;
        case AccountsModel::CurrentPresenceRole:
            return mAccount->currentPresence().status;
        case AccountsModel::CurrentPresenceTypeRole:
            return mAccount->currentPresence().type;
        case AccountsModel::CurrentPresenceStatusMessageRole:
            return mAccount->currentPresence().statusMessage;
        case AccountsModel::RequestedPresenceRole:
            return mAccount->requestedPresence().status;
        case AccountsModel::RequestedPresenceTypeRole:
            return mAccount->requestedPresence().type;
        case AccountsModel::RequestedPresenceStatusMessageRole:
            return mAccount->requestedPresence().statusMessage;
        case AccountsModel::ConnectionStatusRole:
            return mAccount->connectionStatus();
        case AccountsModel::ConnectionStatusReasonRole:
            return mAccount->connectionStatusReason();
        default:
            return QVariant();
    }
}

bool AccountsModelItem::setData(int role, const QVariant &value)
{
    switch (role) {
    case AccountsModel::EnabledRole:
        setEnabled(value.toBool());
        return true;
    case AccountsModel::RequestedPresenceRole:
        setStatus(value.toString());
        return true;
    case AccountsModel::RequestedPresenceStatusMessageRole:
        setStatusMessage(value.toString());
        return true;
    case AccountsModel::NicknameRole:
        setNickname(value.toString());
        return true;
    default:
        return false;
    }
}

void AccountsModelItem::setEnabled(bool value)
{
    mAccount->setEnabled(value);
}

void AccountsModelItem::setStatus(const QString &value)
{
    SimplePresence presence = mAccount->currentPresence();
    presence.status = value;
    mAccount->setRequestedPresence(presence);
}

void AccountsModelItem::setStatusMessage(const QString& value)
{
    SimplePresence presence = mAccount->currentPresence();
    presence.statusMessage = value;
    mAccount->setRequestedPresence(presence);
}

void AccountsModelItem::setNickname(const QString &value)
{
    mAccount->setNickname(value);
}

void AccountsModelItem::setPresence(int type, const QString &status, const QString &statusMessage)
{
    SimplePresence presence;
    presence.type = type;
    presence.status = status;
    presence.statusMessage = statusMessage;
    mAccount->setRequestedPresence(presence);
}

void AccountsModelItem::onRemoved()
{
    int index = parent()->indexOf(this);
    emit childrenRemoved(parent(), index, index);
}

void AccountsModelItem::onChanged()
{
    emit changed(this);
}

void AccountsModelItem::onContactsChanged(const Tp::Contacts &addedContacts,
                                         const Tp::Contacts &removedContacts)
{
    foreach (ContactPtr contact, removedContacts) {
        for (int i = 0; i < mChildren.size(); i++) {
            ContactModelItem *item = qobject_cast<ContactModelItem *>(childAt(i));
            if (item->contact() == contact) {
                emit childrenRemoved(this, i, i);
                break;
            }
        }
    }

    QList<TreeNode *> newNodes;
    foreach (ContactPtr contact, addedContacts) {
        qDebug("contact added: %s", qPrintable(contact->id()));
        newNodes.append(new ContactModelItem(contact));
    }
    emit childrenAdded(this, newNodes);
}

void AccountsModelItem::onStatusChanged(Tp::ConnectionStatus status,
                                       Tp::ConnectionStatusReason statusReason,
                                       const QString &error,
                                       const QVariantMap &errorDetails)
{
    emit connectionStatusChanged(mAccount->uniqueIdentifier(), status, statusReason);
}

void AccountsModelItem::refreshKnownContacts()
{
    //clear the contacts of the account
    emit childrenRemoved(this, 0, size() - 1);

    //reload the known contacts if it has a connection
    QList<TreeNode *> newNodes;
    if (mAccount->haveConnection()) {
        ContactManager *manager = mAccount->connection()->contactManager();
        foreach (ContactPtr contact, manager->allKnownContacts()) {
            newNodes.append(new ContactModelItem(contact));
        }
    }
    emit childrenAdded(this, newNodes);
}

}

