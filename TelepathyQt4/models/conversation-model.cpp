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

#include <TelepathyQt4/models/ConversationModel>

#include "TelepathyQt4/models/_gen/conversation-model.moc.hpp"

#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/ReceivedMessage>
#include <TelepathyQt4/AvatarData>

#include <QPixmap>
#include <QtAlgorithms>

#include "TelepathyQt4/models/ConversationItem"

namespace Tp
{

ConversationModel::ConversationModel(const ContactPtr &self, const TextChannelPtr &channel, QObject *parent)
    : QAbstractListModel(parent),
      mSelf(self),
      mChannel(channel)
{
    connect(mChannel.data(),
            SIGNAL(chatStateChanged(const Tp::ContactPtr&, Tp::ChannelChatState)),
            SLOT(onChatStateChanged(const Tp::ContactPtr&, Tp::ChannelChatState)));

    QHash<int, QByteArray> roles;
    roles[TextRole] = "text";
    roles[ContactRole] = "contact";
    roles[ContactAvatarRole] = "contactAvatar";
    roles[TimeRole] = "time";
    roles[TypeRole] = "type";
    setRoleNames(roles);
}

ConversationModel::~ConversationModel()
{
    qDeleteAll(mItems.begin(), mItems.end());
    mItems.clear();
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
    case ContactRole:
        return item->contact()->alias();
    case ContactAvatarRole:
        return item->contact()->avatarData().fileName;
    case TimeRole:
        return item->time();
    case TypeRole:
        switch (item->type()) {
        case ConversationItem::INCOMING_MESSAGE:
            return QString::fromLatin1("incoming_message");
        case ConversationItem::OUTGOING_MESSAGE:
            return QString::fromLatin1("outgoing_message");
        case ConversationItem::EVENT:
            return QString::fromLatin1("event");
        default:
            return QString();
        }
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
    ConversationItem *item = new ConversationItem(mSelf, QDateTime::currentDateTime(),
                                                  text, ConversationItem::OUTGOING_MESSAGE, this);
    addItem(item);

    mChannel->send(item->text());
}

void ConversationModel::addItem(const ConversationItem *item)
{
    beginInsertRows(QModelIndex(), mItems.count(), mItems.count());
    mItems.append(item);
    endInsertRows();
}

bool ConversationModel::deleteItem(const ConversationItem *item)
{
    int num = mItems.indexOf(item);
    if (num != -1) {
        beginRemoveRows(QModelIndex(), num, num);
        mItems.removeAt(num);
        endRemoveRows();
        return true;
    }

    return false;
}

void ConversationModel::onChannelReady(Tp::PendingOperation *op)
{
}

void ConversationModel::onMessageReceived(const Tp::ReceivedMessage &message)
{
    ConversationItem *item = new ConversationItem(message.sender(), message.sent(),
                                                  message.text(), ConversationItem::INCOMING_MESSAGE, this);
    addItem(item);

    mChannel->acknowledge(QList<Tp::ReceivedMessage>() << message);
}

void ConversationModel::onChatStateChanged(const Tp::ContactPtr &contact, ChannelChatState state)
{
    // ignore events originating from self
    if (contact == mSelf) {
        return;
    }

    if (state == ChannelChatStateGone) {
        QString message = QString::fromLatin1("%1 left the chat").arg(contact->alias());
        ConversationItem *item = new ConversationItem(contact, QDateTime::currentDateTime(), message,
                                                      ConversationItem::EVENT, this);
        addItem(item);
    }
}

/**
  * Disconnect the model from the channel messages queue so that messages on the queue will not
  * be acknowledged and entered into the model automatically
  */
void ConversationModel::disconnectChannelQueue()
{
    disconnect(mChannel.data(),
            SIGNAL(messageReceived(const Tp::ReceivedMessage &)),
            this, SLOT(onMessageReceived(const Tp::ReceivedMessage &)));
}

/**
  * Reconnect the model to the channel queue and acknowledge messages on the queue
  */
void ConversationModel::connectChannelQueue()
{
    //reconnect the signal
    connect(mChannel.data(),
                SIGNAL(messageReceived(const Tp::ReceivedMessage &)),
                SLOT(onMessageReceived(const Tp::ReceivedMessage &)));

    //flush the queue and enter all messages into the model
    // display messages already in queue
    foreach (Tp::ReceivedMessage message, mChannel->messageQueue()) {
        onMessageReceived(message);
    }
}


}

