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

#include "TelepathyQt4/ui/conversation-item.h"

#include "TelepathyQt4/ui/_gen/conversation-item.moc.hpp"

#include <TelepathyQt4/Contact>

namespace Tp {

ConversationItem::ConversationItem(const ContactPtr &contact,
                                   const QDateTime &time,
                                   const QString &text,
                                   Type type,
                                   QObject *parent)
    : QObject(parent),
      mContact(contact),
      mTime(time.isValid() ? time : QDateTime::currentDateTime()),
      mText(text),
      mType(type)
{
}

ConversationItem::~ConversationItem()
{
}

ContactPtr ConversationItem::contact() const
{
    return mContact;
}

QDateTime ConversationItem::time() const
{
    return mTime;
}

QString ConversationItem::text() const
{
    return mText;
}

ConversationItem::Type ConversationItem::type() const
{
    return mType;
}

}

