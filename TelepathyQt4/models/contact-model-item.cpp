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

#include "TelepathyQt4/models/contact-model-item.h"

#include "TelepathyQt4/models/_gen/contact-model-item.moc.hpp"

#include <TelepathyQt4/ContactCapabilities>
#include <TelepathyQt4/ContactManager>

#include <TelepathyQt4/models/AccountsModel>

#include <QImage>

namespace Tp
{

ContactModelItem::ContactModelItem(const Tp::ContactPtr &contact)
    : mContact(contact)
{ 

    connect(contact.data(),
            SIGNAL(aliasChanged(const QString&)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(avatarTokenChanged(const QString&)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(avatarDataChanged(const Tp::AvatarData&)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(simplePresenceChanged(const QString &, uint, const QString&)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(capabilitiesChanged(Tp::ContactCapabilities*)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(locationUpdated(Tp::ContactLocation*)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(infoChanged(const Tp::ContactInfoFieldList&)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(subscriptionStateChanged(Tp::Contact::PresenceState)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(publishStateChanged(Tp::Contact::PresenceState)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(blockStatusChanged(bool)),
            SLOT(onChanged()));
}

QVariant ContactModelItem::data(int role) const
{
    switch(role)
    {
        case AccountsModel::ItemRole:
            return QVariant::fromValue(
                const_cast<QObject *>(
                    static_cast<const QObject *>(this)));
        case AccountsModel::IdRole:
            return mContact->id();
        case Qt::DisplayRole:
        case AccountsModel::AliasRole:
            return mContact->alias();
        case AccountsModel::PresenceStatusRole:
            return mContact->presenceStatus();
        case AccountsModel::PresenceTypeRole:
            return mContact->presenceType();
        case AccountsModel::PresenceMessageRole:
            return mContact->presenceMessage();
        case AccountsModel::SubscriptionStateRole:
            return mContact->subscriptionState();
        case AccountsModel::PublishStateRole:
            return mContact->publishState();
        case AccountsModel::BlockedRole:
            return mContact->isBlocked();
        case AccountsModel::GroupsRole:
            return mContact->groups();
        case AccountsModel::AvatarRole:
            return mContact->avatarData().fileName;
        case Qt::DecorationRole:
            return QImage(mContact->avatarData().fileName);
        case AccountsModel::TextChatCapabilityRole:
            return mContact->capabilities()->supportsTextChats();
        case AccountsModel::MediaCallCapabilityRole:
            return mContact->capabilities()->supportsMediaCalls();
        case AccountsModel::AudioCallCapabilityRole:
            return mContact->capabilities()->supportsAudioCalls();
        case AccountsModel::VideoCallCapabilityRole:
            return mContact->capabilities()->supportsVideoCalls();
        case AccountsModel::UpgradeCallCapabilityRole:
            return mContact->capabilities()->supportsUpgradingCalls();
        default:
            break;
    }

    return QVariant();
}

bool ContactModelItem::setData(int role, const QVariant &value)
{
    switch (role) {
        case AccountsModel::PublishStateRole: {
            Tp::Contact::PresenceState state;
            state = (Tp::Contact::PresenceState) value.toInt();
            switch (state) {
                case Tp::Contact::PresenceStateYes:
                    // authorize the contact and request its presence publication
                    mContact->authorizePresencePublication();
                    mContact->requestPresenceSubscription();
                    return true;
                case Tp::Contact::PresenceStateNo: {
                    // reject the presence publication and remove the contact
                    mContact->removePresencePublication();
                    QList<Tp::ContactPtr> contacts;
                    contacts << mContact;
                    Tp::ContactManager *manager = mContact->manager();
                    // TODO: replace the removeFromGroup code by this line when the models get merged into tp-qt4
                    //mContact->manager()->removeContacts(mContact);
                    foreach (const QString &group, mContact->groups()) {
                        manager->removeContactsFromGroup(group, contacts);
                    }
                    return true;
                }
                default:
                    return false;
            }
        }
        default:
            return false;
    }
}

void ContactModelItem::onChanged()
{
    emit changed(this);
}

}

