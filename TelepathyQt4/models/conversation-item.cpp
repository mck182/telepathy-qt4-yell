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

#include "TelepathyQt4/models/ConversationItem"

#include "TelepathyQt4/models/_gen/conversation-item.moc.hpp"

#include <TelepathyQt4/Contact>

namespace Tp {

struct TELEPATHY_QT4_NO_EXPORT ConversationItem::Private
{
    Private(const ContactPtr &contact,
            const QDateTime &time,
            const QString &text,
            Type type)
        : mContact(contact),
          mTime(time.isValid() ? time : QDateTime::currentDateTime()),
          mText(text),
          mType(type)
    {
    }

    ContactPtr mContact;
    QDateTime mTime;
    QString mText;
    Type mType;
};

ConversationItem::ConversationItem(const ContactPtr &contact,
                                   const QDateTime &time,
                                   const QString &text,
                                   Type type,
                                   QObject *parent)
    : QObject(parent),
      mPriv(new Private(contact, time, text, type))
{
}

ConversationItem::~ConversationItem()
{
    delete mPriv;
}

ContactPtr ConversationItem::contact() const
{
    return mPriv->mContact;
}

QDateTime ConversationItem::time() const
{
    return mPriv->mTime;
}

QString ConversationItem::text() const
{
    return mPriv->mText;
}

ConversationItem::Type ConversationItem::type() const
{
    return mPriv->mType;
}

}
