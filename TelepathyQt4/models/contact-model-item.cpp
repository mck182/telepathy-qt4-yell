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

#include <TelepathyQt4/models/AccountsModel>

#include <QImage>

namespace Tp
{

ContactModelItem::ContactModelItem(const Tp::ContactPtr &contact)
    : mContact(contact)
{ }

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
            return !!mContact->block();
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

}

