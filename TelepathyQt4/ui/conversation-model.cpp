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

#include <TelepathyQt4/PendingReady>

namespace Tp
{

ConversationModel::ConversationModel(const TextChannelPtr &channel)
    : mChannel(channel)
{
    connect(mChannel->becomeReady(),
            SIGNAL(finished(Tp::PendingOperation *)),
            SLOT(onChannelReady(Tp::PendingOperation *)));
}

ConversationModel::~ConversationModel()
{
}

QVariant ConversationModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

int ConversationModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

void ConversationModel::sendMessage(const QString& message)
{
}

void ConversationModel::addItem(const ConversationItem &item)
{
}

void ConversationModel::onChannelReady(PendingOperation *op)
{
}

}

