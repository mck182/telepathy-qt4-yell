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

#include <TelepathyQt4/ui/ConversationModel>
#include "TelepathyQt4/ui/_gen/conversation-model.moc.hpp"

#include <QPixmap>

#include "TelepathyQt4/ui/conversation-item.h"

#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/ReceivedMessage>

namespace Tp
{

ConversationModel::ConversationModel(const TextChannelPtr &channel)
    : mChannel(channel)
{
    Features features;
    features << TextChannel::FeatureMessageQueue
             << TextChannel::FeatureChatState
             << Channel::FeatureCore;
    connect(mChannel->becomeReady(features),
            SIGNAL(finished(Tp::PendingOperation *)),
            SLOT(onChannelReady(Tp::PendingOperation *)));
    connect(mChannel.data(),
            SIGNAL(messageReceived(const Tp::ReceivedMessage &)),
            SLOT(onMessageReceived(const Tp::ReceivedMessage &)));
    connect(mChannel.data(),
            SIGNAL(chatStateChanged(Tp::ContactPtr, ChannelChatState)),
            SLOT(onChatStateChanged(Tp::ContactPtr, ChannelChatState)));
    
    QHash<int, QByteArray> roles;
    roles[TextRole] = "text";
    roles[SenderRole] = "sender";
    roles[SenderAvatarRole] = "senderAvatar";
    roles[TimeRole] = "time";
    setRoleNames(roles);
}

ConversationModel::~ConversationModel()
{
}

QVariant ConversationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= mItems.count()) {
        return QVariant();
    }

    const ConversationItem *item = mItems[index.row()];
    switch (role) {
    case TextRole:
        return item->text();
    case SenderRole:
        return item->sender() ? item->sender()->id() : QString::fromLatin1("You");
    case SenderAvatarRole:
        return item->sender() ? item->sender()->avatarData().fileName : QString();
    case TimeRole:
        return item->time();
    default:
        return QVariant();
    }

}

int ConversationModel::rowCount(const QModelIndex &parent) const
{
    return mItems.count();
}

void ConversationModel::sendMessage(const QString& text)
{
    ConversationItem *item = new ConversationItem(ContactPtr(), QDateTime::currentDateTime(), text, this);
    addItem(item);

    mChannel->send(item->text());
}

void ConversationModel::addItem(const ConversationItem *item)
{
    beginInsertRows(QModelIndex(), mItems.count(), mItems.count());
    mItems.append(item);
    endInsertRows();
}

void ConversationModel::onChannelReady(Tp::PendingOperation *op)
{
}

void ConversationModel::onMessageReceived(const Tp::ReceivedMessage &message)
{
    ConversationItem *item = new ConversationItem(message.sender(), message.sent(), message.text(), this);
    addItem(item);
}

void ConversationModel::onChatStateChanged(const Tp::ContactPtr &contact, ChannelChatState state)
{
    if (state == ChannelChatStateGone) {
        QString message = QString::fromLatin1("left the chat");
        ConversationItem *item = new ConversationItem(contact, QDateTime::currentDateTime(), message);
        addItem(item);
    }
}

}

