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
#include <TelepathyQt4/AvatarData>

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
            SIGNAL(presenceChanged(const Tp::Presence&)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(capabilitiesChanged(const Tp::ContactCapabilities&)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(locationUpdated(const Tp::LocationInfo&)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(infoFieldsChanged(const Tp::Contact::InfoFields&)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(subscriptionStateChanged(Tp::Contact::PresenceState,
                                            const Tp::Channel::GroupMemberChangeDetails&)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(publishStateChanged(Tp::Contact::PresenceState,
                                       const Tp::Channel::GroupMemberChangeDetails&)),
            SLOT(onChanged()));
    connect(contact.data(),
            SIGNAL(blockStatusChanged(bool,
                                      const Tp::Channel::GroupMemberChangeDetails&)),
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
            return mContact->presence().status();
        case AccountsModel::PresenceTypeRole:
            return mContact->presence().type();
        case AccountsModel::PresenceMessageRole:
            return mContact->presence().statusMessage();
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
            return mContact->capabilities().textChats();
        case AccountsModel::MediaCallCapabilityRole:
            return mContact->capabilities().streamedMediaCalls();
        case AccountsModel::AudioCallCapabilityRole:
            return mContact->capabilities().streamedMediaAudioCalls();
        case AccountsModel::VideoCallCapabilityRole:
            return mContact->capabilities().streamedMediaVideoCalls();
        case AccountsModel::UpgradeCallCapabilityRole:
            return mContact->capabilities().upgradingStreamedMediaCalls();
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
                    mContact->manager()->removeContacts(contacts);
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

