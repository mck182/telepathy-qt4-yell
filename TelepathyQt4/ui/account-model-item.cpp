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

#include "account-model-item.h"
#include "TelepathyQt4/ui/_gen/account-model-item.moc.hpp"

#include <TelepathyQt4/Account>
#include <TelepathyQt4/ContactManager>

#include <TelepathyQt4/ui/AccountModel>

#include "contact-model-item.h"

namespace Tp
{

AccountModelItem::AccountModelItem(const AccountPtr &account)
    : mAccount(account)
{
    if (mAccount->haveConnection()) {
        ContactManager *manager = account->connection()->contactManager();
        foreach (ContactPtr contact, manager->allKnownContacts()) {
            addChild(new ContactModelItem(contact));
        }
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
            SIGNAL(haveConnectionChanged(bool)),
            SLOT(onChanged()));
}

QVariant AccountModelItem::data(int role) const
{
    switch (role) {
        case AccountModel::ItemRole:
            return QVariant::fromValue(
                const_cast<QObject *>(
                    static_cast<const QObject *>(this)));
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

bool AccountModelItem::setData(int role, const QVariant &value)
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

void AccountModelItem::setEnabled(bool value)
{
    mAccount->setEnabled(value);
}

void AccountModelItem::setStatus(const QString &value)
{
    SimplePresence presence = mAccount->currentPresence();
    presence.status = value;
    mAccount->setRequestedPresence(presence);
}

void AccountModelItem::setStatusMessage(const QString& value)
{
    SimplePresence presence = mAccount->currentPresence();
    presence.statusMessage = value;
    mAccount->setRequestedPresence(presence);
}

void AccountModelItem::setNickname(const QString &value)
{
    mAccount->setNickname(value);
}

void AccountModelItem::setPresence(int type, const QString &status, const QString &statusMessage)
{
    SimplePresence presence;
    presence.type = type;
    presence.status = status;
    presence.statusMessage = statusMessage;
    mAccount->setRequestedPresence(presence);
}

void AccountModelItem::onChanged()
{
    emit changed(this);
}


void AccountModelItem::onRemoved()
{
    emit removed(this);
}

}

