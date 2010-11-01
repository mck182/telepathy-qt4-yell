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

#include "TelepathyQt4/ui/contact-model-item.h"

#include "TelepathyQt4/ui/_gen/contact-model-item.moc.hpp"

#include <TelepathyQt4/ui/AccountModel>

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
        case AccountModel::ItemRole:
            return QVariant::fromValue(
                const_cast<QObject *>(
                    static_cast<const QObject *>(this)));
        case AccountModel::IdRole:
            return mContact->id();
        case Qt::DisplayRole:
        case AccountModel::AliasRole:
            return mContact->alias();
        case AccountModel::PresenceStatusRole:
            return mContact->presenceStatus();
        case AccountModel::PresenceTypeRole:
            return mContact->presenceType();
        case AccountModel::PresenceMessageRole:
            return mContact->presenceMessage();
        case AccountModel::SubscriptionStateRole:
            return mContact->subscriptionState();
        case AccountModel::PublishStateRole:
            return mContact->publishState();
        case AccountModel::BlockedRole:
            return !!mContact->block();
        case AccountModel::GroupsRole:
            return mContact->groups();
        case AccountModel::AvatarRole:
            return mContact->avatarData().fileName;
        case Qt::DecorationRole:
            return QImage(mContact->avatarData().fileName);
        default:
            break;
    }

    return QVariant();
}

}

